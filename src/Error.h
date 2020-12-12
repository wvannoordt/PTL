#ifndef ERROR_H
#define ERROR_H
#include "PTLOutputStream.h"
#include <iostream>
#include <string>
#include <cstdlib>
#define ErrorKill(mycode) {M_ErrorKill(mycode, __LINE__, __FILE__);}
static inline void M_ErrorKill(std::string message, const int line, const char* file)
{
    PTL::ptlout << "---------------------------------------" << PTL::ptl::endl;
    PTL::ptlout << "Terminate called from file " << file << ", line " << line << ":" << PTL::ptl::endl;
    PTL::ptlout << "Message: " << message << PTL::ptl::endl;
    PTL::ptlout << "---------------------------------------" << PTL::ptl::endl;
    exit(199);
}

#endif
