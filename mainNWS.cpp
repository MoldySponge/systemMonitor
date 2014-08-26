#include <QObject>
#include <QPalette>
#include <QApplication>
#include <QTextBrowser>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

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

/*
 * main mainNWS initializes all variables and starts the program off by
 * constructor initialization from other classes
 */
mainNWS::mainNWS()
{
        tcpfname = "temptcptableinfo";
        udpfname = "tempudptableinfo";
        processfname = "tempprocessinfo";
        hnfname = "temphostnameinfo";
        userfname = "useractions";

        window = new QWidget;
        rpLayout = new QVBoxLayout;
        tcpTLayout = new QVBoxLayout;
        udpTLayout = new QVBoxLayout;
        rssLayout = new QVBoxLayout;
        hnLayout = new QVBoxLayout;
        fullLayout = new QVBoxLayout;        
        topLayout = new QHBoxLayout;        
        botLayout = new QHBoxLayout;
        window->setWindowTitle("Security Monitor");

        udpTableBrowser = new QTextBrowser();
        tcpTableBrowser = new QTextBrowser();
        runningProcessList = new QTextBrowser();
        rssBrowser = new QTextBrowser();
        connectionList = new QTextBrowser();

        rpl = new QLabel("<font color = white><h1>Running Processes</h1></font>");
        ttl = new QLabel("<font color = white><h1>Tcp Table</h1></font>");
        utl = new QLabel("<font color = white><h1>Udp Table</h1></font>");

        rfl = new QLabel("<font color = white><h1>Latest Security News</h1></font>");
        hnl = new QLabel("<font color = white><h1>Current Connected Hostnames</h1></font>");


        QPalette wPal;
        QPalette rPal;

        //sets colors and then calls class methods to start processes
        wPal.setColor(QPalette::Background,Qt::black);
        rPal.setColor(QPalette::Background,Qt::black);
        udp.getUdpTable();
        tcp.getNetworkParameters();
        tcp.getTcpTable();
        processes.GetProcessList();
        rDnsLookup.getHostnameString();

        //connects Qt signals to monitor for changes (Qt signals work a lot like action listeners from Java)
        QObject::connect(&tcp,SIGNAL(tcpChange()),this, SLOT(updateTcpWindow()));
        QObject::connect(&udp,SIGNAL(udpChange()),this,SLOT(updateUdpWindow()));
        QObject::connect(&processes,SIGNAL(runningProcessesChange()),this,SLOT(updateRunningProcessesWindow()));
        QObject::connect(&rss, SIGNAL(rssChange()),this,SLOT(updateRssLabel()));
        QObject::connect(&rDnsLookup,SIGNAL(hostnameChange()),this,SLOT(updateReverseDnsWindow()));
        QObject::connect(&firewallInfo,SIGNAL(wFSChange()),this,SLOT(updateWFStatusLabel()));
  //      QObject::connect(&rssBrowser,SIGNAL(linkClicked()),this,SLOT(writeUserAction()));
        //this next section monitors and updates logs and the window
        udpTable = QString::fromStdString(logReader.readFromLog(udpfname));

        tcpTable = QString::fromStdString(logReader.readFromLog(tcpfname));

        firewallStatus = QString::fromStdString(firewallInfo.getFirewallString());
        firewallLabel = new QLabel(("<font color=\"white\"><h1>" + firewallStatus + "</h1></font>"));

        connectionList->setHtml("<html><head></head><body bgcolor=\"lightgrey\">" +
                                QString::fromStdString(logReader.readFromLog(hnfname) + "</body></html>"));
        connectionList->setMinimumWidth(300);
        connectionList->setMinimumHeight(300);

        rssBrowser->setHtml(("<html><head></head><body bgcolor=\"lightgrey\"><font size=24>" + rss.getRssString() + "</font></body></html>"));
        rssBrowser->setMinimumWidth(500);
        rssBrowser->setMinimumHeight(300);

        runningProcessList->setHtml("<html><head></head><body bgcolor=\"lightgrey\">" +
                                    QString::fromStdString(logReader.readFromLog(processfname) + "</body></html>"));
        runningProcessList->setMinimumWidth(700);
        runningProcessList->setMinimumHeight(300);

        udpTableBrowser->setHtml("<html><head></head><body bgcolor=\"lightgrey\">" +
                                 udpTable + "</body></html>");
        udpTableBrowser->setMinimumWidth(200);
        udpTableBrowser->setMinimumHeight(300);

        tcpTableBrowser->setHtml("<html><head></head><body bgcolor=\"lightgrey\">" +
                                 tcpTable + "</body></html>");
        tcpTableBrowser->setMinimumWidth(200);
        tcpTableBrowser->setMinimumHeight(300);

        //adds everything to the layout Manager for display
        rpLayout->addWidget(rpl);
        rpLayout->addWidget(runningProcessList);
        tcpTLayout->addWidget(ttl);
        tcpTLayout->addWidget(tcpTableBrowser);
        udpTLayout->addWidget(utl);
        udpTLayout->addWidget(udpTableBrowser);

        rssLayout->addWidget(rfl);
        rssLayout->addWidget(rssBrowser);
        hnLayout->addWidget(hnl);
        hnLayout->addWidget(connectionList);

        topLayout->addLayout(rpLayout);
        topLayout->addLayout(tcpTLayout);
        topLayout->addLayout(udpTLayout);

        botLayout->addWidget(firewallLabel);
        botLayout->addLayout(rssLayout);
        botLayout->addLayout(hnLayout);

        fullLayout->addLayout(topLayout); 
        fullLayout->addLayout(botLayout);

        window->setLayout(fullLayout);
        window->setAutoFillBackground(true);
        window->setPalette(wPal);
        window->show();

}

/*
 * emits signals
 */
void mainNWS::updateTcpWindow()
{
    tcpTable = QString::fromStdString(logReader.readFromLog(tcpfname));
    tcpTableBrowser->setHtml("<html><head></head><body bgcolor=\"lightgrey\">" +
                             tcpTable + "</body></html>");
    emit tcpWindowUpdated();
}

void mainNWS::updateUdpWindow()
{
    udpTable = QString::fromStdString(logReader.readFromLog(udpfname));
    udpTableBrowser->setHtml("<html><head></head><body bgcolor=\"lightgrey\">" +
                             udpTable + "</body></html>");
    emit udpWindowUpdated();
}

void mainNWS::updateRunningProcessesWindow()
{
    runningProcessList->setHtml("<html><head></head><body bgcolor=\"lightgrey\">" +
                                QString::fromStdString(logReader.readFromLog(processfname) + "</body></html>"));
    emit runningProcessesWindowUpdated();
}

void mainNWS::updateRssLabel()
{
    rssBrowser->setHtml(("<html><head></head><body bgcolor=\"lightgrey\"><font size=24>" + rss.getRssString() + "</font></body></html>"));
    emit rssLabelUpdated();
}

void mainNWS::updateReverseDnsWindow()
{
    connectionList->setHtml("<html><head></head><body bgcolor=\"lightgrey\">" +
                            QString::fromStdString(logReader.readFromLog(hnfname) + "</body></html>"));
    emit reverseDnsWindowUpdated();
}

void mainNWS::updateWFStatusLabel()
{
    firewallStatus = QString::fromStdString(firewallInfo.getFirewallString());
    firewallLabel->setText(("<font color=\"white\"><h1>" + firewallStatus + "</h1></font>"));
    emit wFStatusLabelUpdated();
}

/*void mainNWS::writeUserAction()
{
    logReader.writeToLog(userfname,"<html><head></head><body><font color=red>"
                         + userLogTime.getCurrentDate()
                         + "</font><br><font color =blue>User clicked a link in Rss Feed</font><br>");
    emit userActionWritten();
}*/
