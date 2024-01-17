#ifndef K_SESSION_H
#define K_SESSION_H

#include <k.h>
#include <stdexcept>
#include <string.h>
#include <iostream>

#define MAX_HOSTNAME_LENGTH 46

namespace kdb {
I handleOk(I handle)
{
    if(handle > 0)
        return 1;
    if(handle == 0)
        fprintf(stderr, "Authentication error %d\n", handle);
    else if(handle == -1)
        fprintf(stderr, "Connection error %d\n", handle);
    else if(handle == -2)
        fprintf(stderr, "Timeout error %d\n", handle);
    return 0;
}


int initSession(char* hostName, I port) {
    if (strlen(hostName) > MAX_HOSTNAME_LENGTH) {
        std::cout<<"Invalid hostname: "<<hostName<<std::endl;
        return -1;
    } else {
        I handle = khp(hostName, port);
        if (handleOk(handle) == 0) {
            return -1;
        } else {std::cout<<"Connection successful: "<<handle<<std::endl;}
        return handle;
}
}

K query(const int handle, const S query) {
    K result = k(handle, query, (K)0);
    if (result->t == -128) {
        std::cerr << "Error in k message: " << result->s << std::endl;
        r0(result); // Release memory
        throw std::runtime_error((std::string)query);
    }
    return result;
}

}

int test_function() {
    K a, b;
    a= ki(3);    // Create K object a
    printf("New K object a has ref count = %d\n", a->r);

    b= r1(a);  // Set b equal to a
    printf("incremented ref count of a = %d\n", a->r);
    printf("ref count of b = %d\n", b->r);

    r0(b); // Decrement b's reference count
    printf("decrement ref count of b = %d\n", b->r);
    printf("ref count of a was decremented to = %d\n", a->r);

    printf("value of a is still available: %d\n", a->i);

    r0(a); // When an object of ref count 0 passed to r0, the object is destroyed 
    return EXIT_SUCCESS;
}

#endif