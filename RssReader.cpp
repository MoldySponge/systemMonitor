#include <iostream>

#include <QtNetwork/QNetworkReply>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>

#include <windows.h>
#include "RssReader.h"

using namespace std;

/*
 * class takes the news from a Security Rss feed
 * implementation for user feed input is planned
 */

//starts the new thread
void MyRssReader::startThread()
{
    _beginthread(MyRssReader::enterThread, 0, this);
}

void MyRssReader::enterThread(void *p)
{
    ((MyRssReader *) p)->threadBody();
    _endthread();
    return;
}
//function is called to read through the threads
void MyRssReader::threadBody()
{
    rotateThroughFeeds();
}

MyRssReader::MyRssReader()
{
    rssIndex = 0;
    manager = new QNetworkAccessManager(this);
    openRssFeed();

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}
//looks for the thread connection to finish and receive a reply
void MyRssReader::replyFinished(QNetworkReply * netReply)
{
    QString str (netReply->readAll());

    /* If we are redirected, try again. TODO: Limit redirection count. */
    QVariant vt = netReply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    delete reply;

    if (!vt.isNull())
    {
        qDebug() << "Redirected to:" << vt.toUrl().toString();
        reply = manager->get(QNetworkRequest(vt.toUrl()));
      //  connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
    }
    else
    {
        /* We have something. */
        QDomDocument doc;
        QString error;
        if (!doc.setContent(str, false, &error))
        {
            rssString = "Error";
        }
        else
        {
            QDomElement docElem = doc.documentElement();
            QDomNodeList nodeList = docElem.elementsByTagName("item");

            for (uint i = 0; i < nodeList.length(); i++)
            {
                QDomNode node = nodeList.item(i);
                QDomElement e = node.toElement();
                QString strTitle = e.elementsByTagName("description").item(0).firstChild().nodeValue();
                rssVect.append(("<font color=\"blue\">" +strTitle + "</font>"));
            }
        }
    }
    rssString = rssVect[rssIndex];
    startThread();
}
//uses a get request to open an rss feed
void MyRssReader::openRssFeed()
{
    reply = manager->get(QNetworkRequest(QUrl("http://www.kaspersky.com/rss/news")));
}
//rotates through the currently known rss feeds
void MyRssReader::rotateThroughFeeds()
{
    for (int i = 1;i<rssVect.length();i++) {
        Sleep(10000);
        rssIndex = i;
        if(i == (rssVect.length()-1))
        {
         i = 0;
         rssIndex = 0;
        }
        rssIndexUpdated();
    }
}
//if the rss feed has changed it emits a signal telling the program
void MyRssReader::rssIndexUpdated()
{
    rssString = rssVect[rssIndex];
    emit rssChange();
}

QString MyRssReader::getRssString()
{
    return rssString;
}


