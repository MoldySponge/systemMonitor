#ifndef REVERSEDNSLOOKUP_H
#define REVERSEDNSLOOKUP_H
#define WIN32_LEAN_AND_MEAN
#include <QObject>
#include <iostream>
#include <process.h>
#include <windows.h>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <vector>

#include "programLogTime.h"
#include "fioLogger.h"
#include "tcpTableAccess.h"
// link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

class ReverseDnsLookup : public QObject
{
    Q_OBJECT

public:
    void startThread();
    void getHostnameString();
public slots:
    void connectionsUpdated();
signals:
    void hostnameChange();
private:
    const char *c;
    string hostnames;
    string hnfname;
    string hnfnametemp;
    ProgramLogTime tpo;
    FioLogger ftpo;


    static void enterThread(void *p);
    void threadBody();
    void recheckConnections();
};

#endif // REVERSEDNSLOOKUP_H
