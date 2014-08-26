
#include <iostream>
#include <string>
#include <time.h>

#include "runningProcesses.h"

using namespace std;

/*
 * returns time
 */
string ProgramLogTime::getCurrentDate()
{
    time(&now);
    timeinfo = localtime(&now);

    return asctime(timeinfo);
}

