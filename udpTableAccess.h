#ifndef UDPTABLEACCESS_H
#define UDPTABLEACCESS_H

#include <QObject>
#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <string>
#include <process.h>
#include <windows.h>

#include "programLogTime.h"
#include "fioLogger.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")


#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

using namespace std;

class UdpTableAccess : public QObject
{
    Q_OBJECT

    public:
        void startThread();
        void getUdpTable();
    public slots:
        void udpTableUpdated();
    signals:
        void udpChange();
    private:
        int numofcon;
        string udpfname;
        string udpfnametemp;
        string numberOfConnections;
        string udpTableInformation;
        ProgramLogTime tpo;
        FioLogger ftpo;

        static void enterThread(void *p);
        void threadBody();
        void watchUdpConnection();
};

#endif
