#pragma once

#include "csvUtils.hpp"
#include "k_session.h"

enum ColumnType { VOLUME, OPEN, HIGH, LOW, CLOSE, UNKNOWN_TYPE };
inline ColumnType mapColumnType(const std::string& columnName) {
    if (columnName == "volume") {
        return VOLUME;
    } else if (columnName == "open") {
        return OPEN;
    } else if (columnName == "high") {
        return HIGH;
    } else if (columnName == "low") {
        return LOW;
    } else if (columnName == "close") {
        return CLOSE;
    } else {
        // Handle unknown column type if necessary
        return UNKNOWN_TYPE; // Add UNKNOWN_TYPE to the enum if needed
    }
}

namespace kdb {
    K parse(const int& handle, const std::string& data) {
            std::vector<std::pair<std::string, std::string>> cols = csv::findCols(data);

    // Create the q table
    std::vector<std::string> colNameList;


    K volumeCol, openCol, highCol, lowCol, closeCol;
for (auto col : cols) {
    std::cout<<col.second<<std::endl;
    if (col.first == "") {
        continue;
    }

    ColumnType colType = mapColumnType(col.first);

    switch (colType) {
        case VOLUME: {
            std::vector<int> tempCol = csv::arrToVec<int>(col.second);
            volumeCol = ktn(KI, tempCol.size());
            for (size_t i = 0; i < tempCol.size(); i++) {
                kI(volumeCol)[i] = tempCol[i];
            }
            std::cout << "Volume Column Type: " << volumeCol->t << ", Num Elements: " << volumeCol->n << std::endl;
            break;
        }
        case OPEN:
        case HIGH:
        case LOW:
        case CLOSE: {
            std::vector<double> tempCol = csv::arrToVec<double>(col.second);
            K& currentCol = (colType == OPEN) ? openCol : (colType == HIGH) ? highCol : (colType == LOW) ? lowCol : closeCol;
            currentCol = ktn(KF, tempCol.size());
            for (size_t i = 0; i < tempCol.size(); i++) {
                kF(currentCol)[i] = tempCol[i];
            }
            std::cout << "Column Type: " << currentCol->t << ", Num Elements: " << currentCol->n << std::endl;
            break;
        }
    }

}

    #ifdef __EXPERIMENTAL__
    K columnValues = knk(5, volumeCol, openCol, highCol, lowCol, closeCol);

    K values_list = knk(5,
        kS(kK(columnValues)[0]), // Assuming volumeCol is a symbol list
        kF(kK(columnValues)[1]), // Assuming openCol is a float list
        kF(kK(columnValues)[2]), // Assuming highCol is a float list
        kF(kK(columnValues)[3]), // Assuming lowCol is a float list
        kF(kK(columnValues)[4])  // Assuming closeCol is a float list
    );
    K k(handle, (S)"values_list:", values_list, (K)0);
    k(handle, (S)"cols_list:", columnValues, (K)0);

    #endif
    
    
    K colList = ktn(KS, 5);
    kS(colList)[0] = ss((S)"VOLUME");
    kS(colList)[1] = ss((S)"OPEN");
    kS(colList)[2] = ss((S)"HIGH");
    kS(colList)[3] = ss((S)"LOW");
    kS(colList)[4] = ss((S)"CLOSE");

    K tickerTable = xD(colList, knk(5, volumeCol, openCol, highCol, lowCol, closeCol));
    r0(volumeCol);
    r0(openCol);
    r0(highCol);
    r0(lowCol);
    r0(closeCol);

    return tickerTable;
    }
}