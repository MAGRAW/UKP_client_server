QT -= gui
QT += core sql
CONFIG += C++11
TEMPLATE = lib

DEFINES += BUILD_TESTS

DESTDIR = $$PWD/../shared/lib

DISTFILES += \
    include/about dbtypes_h.TXT

INCLUDEPATH += $$PWD/include

HEADERS += \
    include/ConnectionManager.h \
    include/dbtypes.h

SOURCES += \
    src/ConnectionManager.cpp
