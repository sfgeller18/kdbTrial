#ifdef __YAHOO__
#include "KDB/k_yahoo.h"
#include "yahoo.hpp"
#endif

#include "KDB/k_session.h"

//Connection Config
#define PORT 12345
#define HOSTNAME "localhost"

int kdbTest(int argc, char* argv[]) {
        std::cout << "KDB+ is enabled" << std::endl;
        int handle = kdb::initSession((S)HOSTNAME, PORT);
    if (handle == -1) {
        std::cout << "Connection failed" << std::endl;
        return EXIT_FAILURE;
    }

    std::string ticker = argv[1];
    std::string startDate = argv[2];
    std::string endDate = argv[3];
    std::string interval = argv[4];

    #ifdef __YAHOO__
    std::string errorBuffer = "";
    yahoo::ticker tick(ticker, startDate, endDate, interval, errorBuffer);

    K result = kdb::query(handle, (S)"2 + 2");
    int resultValue = result->i;
    std::cout << "Result of 2 + 2: " << resultValue << std::endl;

    K tickerTable = tick.toKDB(handle, ticker);
    std::cout<<tickerTable->t<<std::endl;
    K keys= kK(tickerTable)[0];
    K values= kK(tickerTable)[1];
    printf("Keys type = %d\n", keys->t);
    printf("Num keys = %lld\n", keys->n);
        for(int i= 0; i < keys->n; i++) {
        printf("keys[%lld] = %s\n", i, kS(keys)[i]);
    }


    // for(int i = 0; i < std::min(5, (int)values->n); i++) {
    //     printf("Row %d:\n", i + 1);

    //     // Assuming your table has 5 columns
    //     for(int j = 0; j < 5; j++) {
    //         // Adjust the type handling based on the actual types of your columns
    //         switch (kK(values)[j]->t) {
    //             case -KF: // Float column
    //                 printf("    %s: %f\n", kS(keys)[j], kF(kK(values)[j])[i]);
    //                 break;
    //             case -KI: // Int column
    //                 printf("    %s: %d\n", kS(keys)[j], kI(kK(values)[j])[i]);
    //                 break;
    //             // Add cases for other column types as needed
    //             // ...
    //             default:
    //                 break;
    //         }
    //     }
    // }
    r0(tickerTable);
    return 0;
    #endif
}