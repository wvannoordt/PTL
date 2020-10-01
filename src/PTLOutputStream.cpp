#include "PTLOutputStream.h"
namespace PropTreeLib
{
    namespace ptl
    {
        ptloutputflush endl;
    }
    PTLOutputStream ptlout;
    PTLOutputStream::PTLOutputStream(void)
    {
        streams.push_back(&std::cout);
    }
    PTLOutputStream::~PTLOutputStream(void)
    {
        for (size_t i = 0; i < filebufs.size(); i++)
        {
            filebufs[i]->close();
            delete filebufs[i];
        }
        for (size_t i = 1; i < streams.size(); i++)
        {
            delete streams[i];
        }
    }
    void PTLOutputStream::AddFileToStream(std::string filename)
    {
        std::filebuf* next = new std::filebuf();
        filebufs.push_back(next);
        next->open(filename.c_str(), std::ios::out);
        streams.push_back(new std::ostream(next));
    }
    PTLOutputStream& PTLOutputStream::operator << (std::string a)
    {
        for (size_t i = 0; i < streams.size(); i++) *streams[i] << a;
        return *this;
    }
    PTLOutputStream& PTLOutputStream::operator << (double a)
    {
        for (size_t i = 0; i < streams.size(); i++) *streams[i] << a;
        return *this;
    }
    PTLOutputStream& PTLOutputStream::operator << (const char* a)
    {
        for (size_t i = 0; i < streams.size(); i++) *streams[i] << a;
        return *this;
    }
    PTLOutputStream& PTLOutputStream::operator << (char* a)
    {
        for (size_t i = 0; i < streams.size(); i++) *streams[i] << a;
        return *this;
    }
    PTLOutputStream& PTLOutputStream::operator << (int a)
    {
        for (size_t i = 0; i < streams.size(); i++) *streams[i] << a;
        return *this;
    }
    PTLOutputStream& PTLOutputStream::operator << (ptl::ptloutputflush a)
    {
        for (size_t i = 0; i < streams.size(); i++) *streams[i] << std::endl;
        return *this;
    }
    PTLOutputStream& PTLOutputStream::operator << (float a)
    {
        for (size_t i = 0; i < streams.size(); i++) *streams[i] << a;
        return *this;
    }
    PTLOutputStream& PTLOutputStream::operator << (bool a)
    {
        for (size_t i = 0; i < streams.size(); i++) *streams[i] << a;
        return *this;
    }
}