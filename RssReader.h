#ifndef RSSREADER_H
#define RSSREADER_H
#include <iostream>

#include <process.h>
#include <QObject>
#include <QVector>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QtNetwork/QNetworkReply>

#include <windows.h>

using namespace std;
class MyRssReader : public QObject
{
    Q_OBJECT

public:

    MyRssReader();

    void startThread();
public slots:
    void openRssFeed();
    void replyFinished(QNetworkReply*);
    void rssIndexUpdated();
    QString getRssString();

signals:
    void rssChange();

private:
    int rssIndex;
    QString rssString;
    QNetworkAccessManager *manager;
    QNetworkReply * reply;
    QVector<QString> rssVect;

    static void enterThread(void *p);
    void threadBody();
    void rotateThroughFeeds();
};

#endif // RSSREADER_H
