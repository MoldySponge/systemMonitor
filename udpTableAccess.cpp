
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
#include "udpTableAccess.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")


#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

using namespace std;

/*
 *used for tracking udp connections
 */
//starts udp thread
void UdpTableAccess::startThread()
{
    _beginthread(UdpTableAccess::enterThread, 0, this);
}

void UdpTableAccess::enterThread(void *p)
{
    ((UdpTableAccess *) p)->threadBody();
    _endthread();
    return;
}

void UdpTableAccess::threadBody()
{
    watchUdpConnection();
}
//gets the udp table info
void UdpTableAccess::getUdpTable()
{
    FIXED_INFO *pFixedInfo; //object passed to the function
    ULONG ulOutBufLen; //passed to function as well
    //IP_ADDRESS_STRING *pIPAddr; //IP Address to use

    PMIB_UDPSTATS pUDPStats;

    DWORD dwRetVal;
    PMIB_UDPTABLE pUdpTable;
    DWORD dwSize = 0;

    char szLocalAddr[128];

    string udpTableInformation;
    struct in_addr IpAddr;

    pFixedInfo = (FIXED_INFO *)malloc(sizeof(FIXED_INFO));
    ulOutBufLen = sizeof(FIXED_INFO); //correct the buffer size so it can hold the data;

    pUDPStats = (MIB_UDPSTATS *) malloc(sizeof (MIB_UDPSTATS));
    if (pUDPStats == NULL) {
        cout << "Error allocating memory for UDP" << endl;
    }

    if ((dwRetVal = GetUdpStatistics(pUDPStats)) != NO_ERROR) {
        cout << " Getting the ip stats failed with an error" << endl;
    }
    numofcon = pUDPStats->dwOutDatagrams; //monitors datagrams
    numberOfConnections = (string("<font color=darkslategray>Number of Outward Datagrams </font>") + string(std::to_string(pUDPStats->dwOutDatagrams)) + string("<br>"));
    startThread();

    pUdpTable = (MIB_UDPTABLE *) malloc(sizeof (MIB_UDPTABLE));
    if (pUdpTable == NULL) {
        printf("Error allocating memory<br>");

    }


    dwSize = sizeof (MIB_UDPTABLE);
    // Make an initial call to GetTcpTable to
    // get the necessary size into the dwSize variable
    if ((dwRetVal = GetUdpTable(pUdpTable, &dwSize, TRUE)) == ERROR_INSUFFICIENT_BUFFER) {
        free(pUdpTable);
        pUdpTable = (MIB_UDPTABLE *) malloc(dwSize);
        if (pUdpTable == NULL) {
            udpTableInformation = "Error allocating memory</br>";

        }
    }

    // Make a second call to GetTcpTable to get
    // the actual data we require

    if ((dwRetVal = GetUdpTable(pUdpTable, &dwSize, TRUE)) == NO_ERROR) {
        udpTableInformation = "<font color=darkslategray>Number of entries:</font> " + string(std::to_string((int)pUdpTable->dwNumEntries)) + "<br><br>";

        for (int i = 0; i < (int)pUdpTable->dwNumEntries; i++) {
            IpAddr.S_un.S_addr = (u_long)pUdpTable->table[i].dwLocalAddr;
            strcpy_s(szLocalAddr, sizeof (szLocalAddr), inet_ntoa(IpAddr));

            udpTableInformation += string("<font color = green>UDP Local Addr:</font> ") + string(szLocalAddr) + "<br>";
            udpTableInformation += string("<font color = green>UDP Local Port:</font>  ") + std::to_string(ntohs((u_short)pUdpTable->table[i].dwLocalPort)) + "<br>";
            udpTableInformation += "<br>";
        }
    }
    else {
        udpTableInformation += string("GetUdpTable failed with <br>") + std::to_string(dwRetVal);
        free(pUdpTable);

    }

    if (pUdpTable != NULL) {
        free(pUdpTable);
        pUdpTable = NULL;
    }

    udpfname = "udptableinfo";
    udpfnametemp = "tempudptableinfo";
    ftpo.writeToTempLog(udpfnametemp,(("<font color =red><h2>" + tpo.getCurrentDate() + "</h2></font><br><br>" + (numberOfConnections += udpTableInformation))) );
    ftpo.writeToLog(udpfname,("<font color=red><h2>" + tpo.getCurrentDate() + "</h2></font><br><br>" + (numberOfConnections += udpTableInformation)));
}
/*watches the connection for changes in datagrams
*sleeps for a while so as not to overwhelm the disk
*currently there is a lot of disk writing in the program
*optimally this will change in the future to store data and then write
*to disk later.
* One of the reasons for this was to prevent alt-f4 closes
* which would prevent the log from being written and information would
* be lost and not recorded
*/
void UdpTableAccess::watchUdpConnection()
{
    bool exit = true;
    while(exit){
        Sleep(5000);
        FIXED_INFO *pFixedInfot; //object passed to the function
        ULONG ulOutBufLent; //passed to function as well
        //IP_ADDRESS_STRING *pIPAddr; //IP Address to use

        PMIB_UDPSTATS pUDPStatst;

        DWORD dwRetValt;

        pFixedInfot = (FIXED_INFO *)malloc(sizeof(FIXED_INFO));
        ulOutBufLent = sizeof(FIXED_INFO); //correct the buffer size so it can hold the data;

        pUDPStatst = (MIB_UDPSTATS *) malloc(sizeof (MIB_UDPSTATS));
        if (pUDPStatst == NULL) {
            cout << "Error allocating memory for UDP" << endl;
        }

        if ((dwRetValt = GetUdpStatistics(pUDPStatst)) != NO_ERROR) {
            cout << " Getting the ip stats failed with an error" << endl;
        }

        if(numofcon != pUDPStatst->dwOutDatagrams)
        {
            exit = false;
            getUdpTable();
            udpTableUpdated();
        }
    }    
}
//emits when changes are made to udp table
void UdpTableAccess::udpTableUpdated()
{
    emit udpChange();
}

