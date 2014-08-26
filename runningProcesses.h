#ifndef RUNNINGPROCESSES_H
#define RUNNINGPROCESSES_H

#include <QObject>
#include <windows.h>
#include <sstream>
#include <string>
#include <tlhelp32.h>
#include <stdio.h>
#include <process.h>
#include <stdlib.h>
#include <assert.h>

#include "programLogTime.h"
#include "fioLogger.h"

#define BUFFER_SIZE 100

class RunningProcesses : public QObject
{
    Q_OBJECT

public:
    string processes;
    string processfname;
    string processfnametemp;
    BOOL GetProcessList();
    BOOL ListProcessModules( DWORD dwPID );

    void printError( TCHAR* msg );
    void startThread();
public slots:
    void runningProcessesUpdated();
signals:
    void runningProcessesChange();

private:
    char str[260];
    int ret;
    char    *pMBBuffer2;
    stringstream ss;
    string curString;
    ProgramLogTime tpo;
    FioLogger ftpo;

    size_t   i;
    char *pMBBuffer;


    static void enterThread(void *p);
    void threadBody();
    void convertWChar();
};

// Convert a wide Unicode string to an UTF8 string

#endif
