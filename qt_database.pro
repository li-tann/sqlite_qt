QT -= gui
QT += sql

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/../bin/debug
    OBJECTS_DIR = ./debug
}else{
    DESTDIR = $$PWD/../bin/release
    OBJECTS_DIR = ./release
}

SOURCES += \
        main.cpp \
    database_test.cpp \
    tinyxml2.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    database_test.h \
    tinyxml2.h
