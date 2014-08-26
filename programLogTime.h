#ifndef PROGRAMLOGTIME_H
#define PROGRAMLOGTIME_H

#include <iostream>
#include <string>
#include <time.h>

using namespace std;

class ProgramLogTime
{
public:
    string getCurrentDate();
private:
    time_t now;
    struct tm * timeinfo;
};

#endif
