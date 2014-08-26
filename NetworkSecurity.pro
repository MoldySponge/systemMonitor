QT += gui core webkit network xml widgets webkitwidgets

DEFINES += NOMINMAX
SOURCES += \
    main.cpp \
    mainNWS.cpp \
    tcpTableAccess.cpp \
    udpTableAccess.cpp \
    wfStatusAccess.cpp \
    runningProcesses.cpp \
    programLogTime.cpp \
    fioLogger.cpp \
    RssReader.cpp \
    reverseDnsLookup.cpp

HEADERS += \
    tcpTableAccess.h \
    udpTableAccess.h \
    wfStatusAccess.h \
    runningProcesses.h \
    fioLogger.h \
    programLogTime.h \
    mainNWS.h \
    RssReader.h \
    reverseDnsLookup.h
