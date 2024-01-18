#pragma once

#include <sstream>
#include <vector>

namespace csv {
template <typename T>
std::vector<T> arrToVec(const std::string& str) {
    std::string temp = "";
    std::vector<T> result;
    if (str[0] != '[') {
        return result;
    }
    for (size_t i = 1; i < str.size(); i++) {
        char c = str[i];
        while (str[i] != ',' && str[i] != ']') {
            temp += str[i];
            i++;
        }
        if (str[i] == ',' || str[i] == ']') {
            std::istringstream iss(temp);
            T numericValue;
            if (iss >> numericValue) {
                result.push_back(numericValue);
            }
            temp = "";
        }
    }
    return result;
}

std::vector<std::pair<std::string, std::string>> findCols(const std::string& str) {
    std::vector<std::pair<std::string, std::string>> result;
    std::string tempColName = "";
    std::string tempArr = "";
    bool colNameFlag = false;
    bool dataFlag = false;

    for (size_t i = 2; i < str.size(); i++) {
        if (str[i] == '"') {
            i++;
            colNameFlag = !colNameFlag;
        }

        if (colNameFlag) {
            tempColName += str[i];
        } else if (str[i] == '[') {
            tempArr += str[i];
            dataFlag = true;
        } else if (str[i] == ']') {
            tempArr += str[i];
            dataFlag = false;
            result.push_back(std::make_pair(tempColName, tempArr));
            tempColName = "";
            tempArr = "";
        } else if (dataFlag) {
            tempArr += str[i];
        }
    }

    return result;
}



}
