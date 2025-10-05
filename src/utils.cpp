#include "utils.h"

void error(string desc) {
    cerr << "ERROR: " << desc << endl;
    exit(1);
}

void error(string desc,filesystem::path file,int line) {
    cerr << "ERROR FILE: " << file << "LINE: " << line << " : " << desc << endl;
    exit(1);
}

void warn(string desc) {
    cout << "WARNING: " << desc << endl;
}

void warn(string desc,filesystem::path file,int line) {
    cerr << "WARNING FILE: " << file << "LINE: " << line << " : " << desc << endl;
}

void todo(string desc,filesystem::path file,int line) {
    cerr << "TODO FILE: " << file << "LINE: " << line << " : " << desc << endl;
    exit(1);
}