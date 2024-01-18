#include "common.h"

//Connection Config
#define PORT 12345
#define HOSTNAME "localhost"


int main(int argc, char* argv[]) {
    #ifdef __KDB__
    std::cout << "KDB+ is enabled" << std::endl;
        int handle = kdb::initSession(HOSTNAME, PORT);
    if (handle == -1) {
        std::cout << "Connection failed" << std::endl;
        return EXIT_FAILURE;
    }

    K result = kdb::query(handle, (S)"2 + 2");
    int resultValue = result->i;
    std::cout << "Result of 2 + 2: " << resultValue << std::endl;

    #endif

    #ifdef __YAHOO__
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
    #ifdef __KDB__
    K tickerTable = tick.toKDB("test");
    r0(tickerTable);
    #endif
    #endif



    return 0;
}