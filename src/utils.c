#include "utils.h"

// ---------------------
//  Logging
// \/-------------------

void error(const char* msg){
    printf("ERROR: %s\n",msg);
    exit(1);
}

void warning(const char* msg){
    printf("WARNING: %s\n",msg);
}

void todo(const char* msg){
    printf("TODO: %s\n",msg);
    exit(1);
}

// ---------------------
//  Asserts
// \/-------------------

void errorz(int val, const char* msg){
    if(val == 0){
        printf("ERROR: %s\n",msg);
        exit(1);
    }
}

void errornz(int val, const char* msg){
    if(val != 0){
        printf("ERROR: %s\n",msg);
        exit(1);
    }
}

void errorpz(void* val, const char* msg){
    if(val == 0){
        printf("ERROR: %s\n",msg);
        exit(1);
    }
}

void errorpnz(void* val, const char* msg){
    if(val != 0){
        printf("ERROR: %s\n",msg);
        exit(1);
    }
}
