#include "common.h"


int main(int argc, char* argv[]) {
    #ifdef __KDB__
    int a = kdbTest(argc, argv);
    #endif

    #ifdef __YAHOO__
    int b = yahooTest(argc, argv);
    #endif

    return (a || b);
}