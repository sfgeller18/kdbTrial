#ifndef YAHOO_TO_STRING_HPP
#define YAHOO_TO_STRING_HPP

#include <string>
#include <curl/curl.h>
#include <timeUtils.h>
#include <vector>
#include <fstream>
#include <csvUtils.hpp>

#ifdef __KDB__
#include <KDB/k_yahoo.h>
#endif

#ifdef __JSON__
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#endif

#define YAHOO_HEADER_LEN 587


namespace yahoo {

class Message {
public:
    Message(const std::string& url);
    Message() = default;
    const std::string& getBody() const { return body; }

private:
    CURL* initCurl(const std::string& url);
    static size_t writeCallback(char* content, size_t size, size_t nmemb, void* userdata);

    std::string body;
};

Message::Message(const std::string& url) {
    CURL* curl = initCurl(url);
    if (curl) {
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            // Handle error or throw an exception
            // For simplicity, let's set body to an error message
            body = "Error in curl_easy_perform: " + std::string(curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }
}

CURL* Message::initCurl(const std::string& url) {
    CURL* curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
    }
    return curl;
}

size_t Message::writeCallback(char* content, size_t size, size_t nmemb, void* userdata) {
    Message* message = static_cast<Message*>(userdata);
    if (message) {
        message->body.append(content, size * nmemb);
    }
    return size * nmemb;
}

class ticker {
public:
    ticker(std::string _symbol, std::string _startDate, std::string _endDate, std::string _interval, std::string _errorBuffer);
    #ifdef __JSON__
    json stockParser();
    void saveCSV(const std::string& filePath);
    #endif
    #ifdef __KDB__
    K toKDB(const int& handle, const std::string& tableName);
    #endif

    std::string getData() {
        return message.getBody();
    }
private:
    std::string symbol;
    std::string startDate;
    std::string endDate;
    std::string interval;
    std::string errorBuffer;  // You may want to consider removing this and using exceptions for error handling
    std::string responseBuffer;
    Message message;
};

ticker::ticker(std::string _symbol, std::string _startDate, std::string _endDate, std::string _interval, std::string _errorBuffer)
    : symbol(_symbol), startDate(timestamper(_startDate)), endDate(timestamper(_endDate)), interval(_interval), errorBuffer(_errorBuffer) {
    
    std::string url = "https://query1.finance.yahoo.com/v8/finance/chart/"
                    + symbol
                    + "?symbol="
                    + symbol
                    + "&period1=" + startDate
                    + "&period2=" + endDate
                    + "&interval=" + interval;

    // Initialize message directly in the constructor body
    message = Message(url);
    }

#ifdef __JSON__
json ticker::stockParser() {
    json jsonData;
    json quotes;
    try {
        jsonData = json::parse(message.getBody());

        // Access the "indicators" array under "chart" -> "result"
        if (jsonData.contains("chart") && jsonData["chart"].contains("result")) {
            if (!jsonData["chart"]["result"].empty()) {
                quotes = jsonData["chart"]["result"][0]["indicators"]["quote"][0];

                // Check if "timestamps" array exists
                if (jsonData["chart"]["result"][0].contains("timestamp")) {
                    const json& timestamps = jsonData["chart"]["result"][0]["timestamp"];
                    quotes["timestamp"] = timestamps;
                }
            }
        }
    } catch (const json::parse_error& e) {
        errorBuffer = "Invalid Query: " + std::string(e.what());
    }

    // Return the modified quotes or an empty JSON object
    return quotes;
}

void ticker::saveCSV(const std::string& filePath) {
    std::ofstream csvFile(filePath);
    json jsonData = this->stockParser();
    try {
        if (!csvFile.is_open()) {
            throw std::runtime_error("Error: Unable to open file for writing: " + filePath);
        }

        csvFile << "timestamp, close,low,volume,open,high" << std::endl;

        auto& timestampArray = jsonData["timestamp"];
        auto& closeArray = jsonData["close"];
        auto& lowArray = jsonData["low"];
        auto& volumeArray = jsonData["volume"];
        auto& openArray = jsonData["open"];
        auto& highArray = jsonData["high"];

        size_t arraySize = closeArray.size();

        // Iterate over indices and print values to the CSV file
        for (size_t i = 0; i < arraySize; ++i) {
            csvFile << timestampArray[i] << ", " << closeArray[i] << ","
                    << lowArray[i] << ","
                    << volumeArray[i] << ","
                    << openArray[i] << ","
                    << highArray[i] << std::endl;
        }

                    csvFile << typeid(timestampArray[0]).name() << ", " << typeid(closeArray[0]).name() << ","
                    << typeid(lowArray[0]).name() << ","
                    << typeid(volumeArray[0]).name() << ","
                    << typeid(openArray[0]).name() << ","
                    << typeid(highArray[0]).name() << ","
                    << typeid(closeArray[0]).name() << std::endl;

        // Check for any writing errors
        if (!csvFile.good()) {
            throw std::runtime_error("Error: Writing to CSV file failed.");
            errorBuffer = "Error: Writing to CSV file failed.";
        }
    } catch (const std::exception& e) {
         errorBuffer = "Exception caught: " + std::string(e.what()); 
    } catch (...) {
        errorBuffer = "Unknown Exception caught in printObj "; 
    }
}
#endif

#ifdef __KDB__
K ticker::toKDB(const int& handle, const std::string& tableName) {
    std::string body = message.getBody();
    size_t index1 = body.find("timestamp") + 11;
    size_t index2 = body.find("],\"");
    size_t index3 = body.find("quote") + 7;
    size_t index4 = body.find("]}]") + 2;

    // Correct the variable name to use index1 for substring
    std::string timestamps = body.substr(index1, index2 - index1);
    timestamps += "]";
    std::vector<std::string> timestampArray = csv::arrToVec<std::string>(timestamps);

    std::string data = body.substr(index3, index4 - index3 + 1);

    return kdb::parse(handle, data);
}
#endif

}
#endif