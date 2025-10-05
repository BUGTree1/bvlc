#ifndef __UTILS_H__
#define __UTILS_H__

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

using namespace std;

void error(string desc);
void error(string desc,const char* file,int line);
void warn(string desc);
void warn(string desc,const char* file,int line);
void todo(string desc,const char* file,int line);

template<typename T>
T errorz(T val,string desc){
    if (val == 0) error(desc);
    return val;
}

template<typename T>
T errornz(T val,string desc){
    if (val != 0) error(desc);
    return val;
}

#endif // __UTILS_H__