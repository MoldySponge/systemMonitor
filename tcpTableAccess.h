#ifndef TCPTABLEACCESS_H
#define TCPTABLEACCESS_H

#include <QObject>

#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <string>
#include <process.h>
#include <windows.h>
#include <vector>

#include "programLogTime.h"
#include "fioLogger.h"


#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")


#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

using namespace std;

class TcpTableAccess : public QObject
{
    Q_OBJECT

    public:
        void startThread();
        void getTcpTable();
        void getNetworkParameters();
        static vector<string> addressVector;

    public slots:
        void tcpTableUpdated();

    signals:
        void tcpChange();
    private:
        int numofcon;
        string tcpfname;
        string tcpfnametemp;
        string numberOfConnections;
        string tcpTableInformation;

        ProgramLogTime tpo;
        FioLogger ftpo;


       static void enterThread(void *p);
       void threadBody();
       void watchTcpConnection();

};


#endif
