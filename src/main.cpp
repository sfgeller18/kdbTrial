#include "common.h"

//Connection Config
#define PORT 12345
#define HOSTNAME "localhost"



  int main() {
    #ifdef __KDB__
    std::cout<<"KDB+ is enabled"<<std::endl;
    #endif

    #ifdef __YAHOO__
    std::cout<<"Yahoo is enabled"<<std::endl;
    #endif

    int handle = kdb::initSession(HOSTNAME, PORT);
    if (handle == -1) {
        std::cout<<"Connection failed"<<std::endl;
        return EXIT_FAILURE;
    }
    K result = kdb::query(handle, (S)"2 + 2");
    int resultValue = result->i;
    std::cout << "Result of 2 + 2: " << resultValue << std::endl;
    std::string errorBuffer = "";
    std::string query = GetPrices("AAPL", "2021-03-01", "2021-03-31", "1d", errorBuffer);
    std::cout << "Result of GetPrices: " << query << std::endl; 
    return 0;

}