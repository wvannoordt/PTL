#ifndef ERROR_H
#define ERROR_H
#include "PTLOutputStream.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <exception>
#define ErrorKill(mycode) {M_ErrorKill(mycode, __LINE__, __FILE__);}
namespace PTL
{
    struct PTLException : public std::exception
    {
        PTLException(std::string message_in, std::string file_in, int lineNumber_in)
        { 
            message = message_in;
            file = file_in;
            lineNumber = lineNumber_in;
            std::string line = "---------------------------------------";
            completeMessage = "\n";
            completeMessage += line;
            completeMessage += "\nPTLException thrown.\nMessage:\n";
            completeMessage += message;
            completeMessage += "\nFile: ";
            completeMessage += file;
            completeMessage += "\nLine: ";
            completeMessage += std::to_string(lineNumber);
            completeMessage += "\n";
            completeMessage += line;
        }
        const char* what() const throw()
        {
        	return completeMessage.c_str();
        }
        std::string file, message, completeMessage;
        int lineNumber;
    };
}
static inline void M_ErrorKill(std::string message, const int line, const char* file)
{
    throw PTL::PTLException(message, file, line);
}

#endif
