#include "QueryResult.h"
#include "PTL.h"
#include "PTLc.h"
namespace PTL
{
    void QueryResult::KillIfNotTerminal(void)
    {
        if (isSection) ErrorKill("Attempted to access a sections content for parsing as a primitive!");
    }
    
    QueryResult::operator const std::string()
    {
        KillIfNotTerminal();
        PTLString pvar("", "");
        std::string output;
        if (!pvar.ParseFromString(content, &output) && crash_on_missing_option) ErrorKill("Cannot parse \"" + content + "\" as string!");
        return output;
    }
    
    QueryResult::operator int ()
    {
        KillIfNotTerminal();
        PTLInteger pvar(0, "");
        int output;
        if (!pvar.ParseFromString(content, &output) && crash_on_missing_option) ErrorKill("Cannot parse \"" + content + "\" as integer!");
        return output;
    }
    
    QueryResult::operator std::vector<std::string> ()
    {
        KillIfNotTerminal();
        ErrorKill("QueryResult::operator std::vector<std::string> () not yet implemented");
        return std::vector<std::string> ();
    }
    
    QueryResult::operator double ()
    {
        KillIfNotTerminal();
        PTLDouble pvar(0, "");
        double output;
        if (!pvar.ParseFromString(content, &output)  && crash_on_missing_option) ErrorKill("Cannot parse \"" + content + "\" as double!");
        return output;
    }
    
    QueryResult::operator float ()
    {
        KillIfNotTerminal();
        PTLDouble pvar(0, "");
        double output;
        if (!pvar.ParseFromString(content, &output) && crash_on_missing_option) ErrorKill("Cannot parse \"" + content + "\" as double!");
        return (float)output;
    }
    
    QueryResult::operator bool ()
    {
        KillIfNotTerminal();
        PTLBoolean pvar(0, "");
        bool output;
        if (!pvar.ParseFromString(content, &output) && crash_on_missing_option) ErrorKill("Cannot parse \"" + content + "\" as boolean!");
        return output;
    }
    
    QueryResult::operator std::vector<int> ()
    {
        KillIfNotTerminal();
        PTLDynamicIntegerArray pvar("");
        int* outputar;
        int outputNum;
        if (!pvar.ParseFromString(content, &outputar) && crash_on_missing_option) ErrorKill("Cannot parse \"" + content + "\" as integer array!");
        pvar.SetSecondaryVariable(&outputNum);
        std::vector<int> output;
        for (int i = 0; i < outputNum; i++) output.push_back(outputar[i]);
        return output;
    }
    
    QueryResult::operator std::vector<double> ()
    {
        KillIfNotTerminal();
        PTLDynamicDoubleArray pvar("");
        double* outputar;
        int outputNum;
        if (!pvar.ParseFromString(content, &outputar) && crash_on_missing_option) ErrorKill("Cannot parse \"" + content + "\" as double array!");
        pvar.SetSecondaryVariable(&outputNum);
        std::vector<double> output;
        for (int i = 0; i < outputNum; i++) output.push_back(outputar[i]);
        return output;
    }
}