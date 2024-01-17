#ifndef YAHOO_TO_STRING_HPP
#define YAHOO_TO_STRING_HPP

#include <string>
#include <curl/curl.h>
#include <timeUtils.h>

size_t writeCallback(char *content, size_t size, size_t nmemb, void *userdata) {
    // Append the content to user data
    ((std::string*)userdata)->append(content, size * nmemb);

    // Return the real content size
    return size * nmemb;
}

std::string GetPrices(const std::string& symbol, const std::string& startDate, std::string endDate, std::string interval, std::string& errorBuffer) {

        if (startDate == endDate) {return "";}

        std::string ss1 = timestamper(startDate); 
        std::string ss2 = timestamper(endDate); 
        
        std::string url = "https://query1.finance.yahoo.com/v8/finance/chart/"
                + symbol
                + "?symbol="
                + symbol
                + "&period1=" + ss1
                + "&period2=" + ss2
                + "&interval=" + interval;


        CURL* curl = curl_easy_init();
        std::string responseBuffer;

        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

            // Write result into the buffer
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);

            // Perform the request
            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                errorBuffer = "Error in curl_easy_perform: " + std::string(curl_easy_strerror(res));
                // Handle the error, possibly by returning an empty string or throwing an exception
                curl_easy_cleanup(curl);
                return "";
            }
            // Cleanup
            curl_easy_cleanup(curl);
        }
        return responseBuffer;
    } 

#endif