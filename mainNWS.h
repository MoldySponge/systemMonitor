#ifndef MAINNWS_H
#define MAINNWS_H

#include <QObject>
#include <QApplication>
#include <QTextBrowser>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>

#include <iostream>

#include <string>


#include "tcpTableAccess.h"
#include "udpTableAccess.h"
#include "wfStatusAccess.h"
#include "runningProcesses.h"
#include "fioLogger.h"
#include "mainNWS.h"
#include "RssReader.h"
#include "reverseDnsLookup.h"

class mainNWS : public QObject
{
    Q_OBJECT
public:
    mainNWS();

public slots:
    void updateTcpWindow();
    void updateUdpWindow();
    void updateRunningProcessesWindow();
    void updateRssLabel();
    void updateReverseDnsWindow();
    void updateWFStatusLabel();
  //  void writeUserAction();

signals:
    void tcpWindowUpdated();
    void udpWindowUpdated();
    void runningProcessesWindowUpdated();
    void rssLabelUpdated();
    void reverseDnsWindowUpdated();
    void wFStatusLabelUpdated();
  //  void userActionWritten();

private:

    QString tcpTable;
    QString udpTable;
    QString firewallStatus;

    string tcpfname;
    string udpfname;
    string processfname;
    string hnfname;
    string userfname;


    FioLogger logReader;
    QWidget *window;



    QVBoxLayout *rpLayout;
    QVBoxLayout *tcpTLayout;
    QVBoxLayout *udpTLayout;
    QVBoxLayout *rssLayout;
    QVBoxLayout *hnLayout;
    QHBoxLayout *topLayout;  
    QHBoxLayout *botLayout;
    QVBoxLayout *fullLayout;

    QLabel *firewallLabel;
    QLabel *rpl;
    QLabel *ttl;
    QLabel *utl;
    QLabel *rfl;
    QLabel *hnl;
    QTextBrowser *rssBrowser;
    QTextBrowser *tcpTableBrowser;
    QTextBrowser *udpTableBrowser;
    QTextBrowser *runningProcessList;
    QTextBrowser *connectionList;

    TcpTableAccess tcp;
    UdpTableAccess udp;
    WFStatus firewallInfo;
    RunningProcesses processes;
    MyRssReader rss;
    ReverseDnsLookup rDnsLookup;
    ProgramLogTime userLogTime;

};



#endif // MAINNWS_H
