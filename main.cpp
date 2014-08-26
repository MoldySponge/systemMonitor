#include <QApplication>
#include <QTextBrowser>
#include <QHBoxLayout>
#include <QLabel>

#include "tcpTableAccess.h"
#include "udpTableAccess.h"
#include "wfStatusAccess.h"
#include "runningProcesses.h"
#include "fioLogger.h"
#include "mainNWS.h"
#include "RssReader.h"

/*
 * used for starting up the main program
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    mainNWS *tpo = new mainNWS();

    return app.exec();
}
