#ifndef OUTPUTSTREAM_H
#define OUTPUTSTREAM_H
#include <vector>
#include <ostream>
#include <iostream>
#include <fstream>
namespace PTL
{
    namespace ptl
    {
        class ptloutputflush
        {
            public:
                friend std::ostream & operator << (std::ostream &out, const ptloutputflush &c) {out << std::endl; return out;}
        };
        extern ptloutputflush endl;
    }
    class PTLOutputStream
    {
        public:
            PTLOutputStream(void);
            ~PTLOutputStream(void);
            void AddFileToStream(std::string filename);
            PTLOutputStream& operator << (std::string a);
            PTLOutputStream& operator << (double a);
            PTLOutputStream& operator << (const char* a);
            PTLOutputStream& operator << (char* a);
            PTLOutputStream& operator << (int a);
            PTLOutputStream& operator << (ptl::ptloutputflush a);
            PTLOutputStream& operator << (float a);
            PTLOutputStream& operator << (bool a);
        private:
            std::vector<std::ostream*> streams;
            std::vector<std::filebuf*> filebufs;
    };
    extern PTLOutputStream ptlout;
}
#endif