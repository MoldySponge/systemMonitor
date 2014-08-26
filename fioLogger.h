#ifndef FIOLOGGER_H
#define FIOLOGGER_H

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>

using namespace std;

class FioLogger
{
public:

    string readFromLog(string name);
    void writeToLog(string name, string &info);
    void writeToTempLog(string name, string &info);
private:
    FILE *writer;
    FILE *twriter;
};


#endif
