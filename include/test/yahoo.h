#include <iostream>
#include <string>
#include "yahoo.hpp"

#ifdef __JSON__
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#endif

int yahooTest(int argc, char* argv[]) {
        std::cout << "Yahoo is enabled" << std::endl;
        // Check the number of command line arguments
    if (argc < 5) {
        std::cout << "Usage: " << argv[0] << " <ticker> <start_date> <end_date> <interval>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string ticker = argv[1];
    std::string startDate = argv[2];
    std::string endDate = argv[3];
    std::string interval = argv[4];

    std::string errorBuffer = "";
    yahoo::ticker tick(ticker, startDate, endDate, interval, errorBuffer);
    #ifdef __JSON__
    tick.stockParser();
    std::cout<<tick.getData()<<std::endl;
    tick.saveCSV("../data/test.csv");
    #endif
    return 0;
}