#ifndef ERROR_H
#define ERROR_H
#include "PTLOutputStream.h"
#include <iostream>
#include <string>
#include <cstdlib>
#define ErrorKill(mycode) {M_ErrorKill(mycode, __LINE__, __FILE__);}
static inline void M_ErrorKill(std::string message, const int line, const char* file)
{
    PropTreeLib::ptlout << "---------------------------------------" << PropTreeLib::ptl::endl;
    PropTreeLib::ptlout << "Terminate called from file " << file << ", line " << line << ":" << PropTreeLib::ptl::endl;
    PropTreeLib::ptlout << "Message: " << message << PropTreeLib::ptl::endl;
    PropTreeLib::ptlout << "---------------------------------------" << PropTreeLib::ptl::endl;
    exit(199);
}

#endif
