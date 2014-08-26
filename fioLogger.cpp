#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>

#include "fioLogger.h"
/*
 * reads data from the specified log
 *
 */
string FioLogger::readFromLog(string name)
{
    name += ".html";
    string curData;
    string data = "";
    ifstream input(name.c_str());
    while(!input.eof()) {
        getline(input, curData);
        data += " \n" + curData;
    }
    input.close();
    return data;
}
/*
 * writes data to the specified log
 */
void FioLogger::writeToLog(string name, string &info)
{
    name += ".html";
    writer = fopen(name.c_str(), "a");
    fprintf(writer, info.c_str());
    fflush(writer);
    fclose(writer);

}
/*
 * temporary logs are for display purposes currently
 */
void FioLogger::writeToTempLog(string name, string &info)
{
    name += ".html";
    twriter = fopen(name.c_str(), "w");
    fprintf(twriter, info.c_str());
    fflush(twriter);
    fclose(twriter);
}
using namespace std;

