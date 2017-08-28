#-------------------------------------------------
#
# Project created by QtCreator 2017-03-07T16:25:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = faceDAI
TEMPLATE = app

INCLUDEPATH += /usr/local/include

SOURCES += main.cpp\
        facerecognize.cpp \
        global.cpp \
        face_database_api.cpp \
    client_network.cpp

HEADERS  += facerecognize.h \
        global.h \
        face_database_api.h \
    client_network.h

FORMS    += facerecognize.ui


LIBS    += -L./newlibs/ \
       -lseeta_facedet_lib -lseeta_fa_lib -lviplnet

LIBS    += -L /usr/local/lib/ \
        -lopencv_core -lopencv_features2d \
        -lopencv_highgui -lopencv_imgproc \
        -lopencv_ml -lopencv_video -lopencv_contrib \
        -lopencv_objdetect

LIBS    += -L /usr/local/lib/ \
        -lsqlite3


QMAKE_CXXFLAGS +=-msse4.2 -std=c++11 -O2 -w -static -lsqlite3 -lpthread


DISTFILES += \
    README.md \
    configure/configure.json

RESOURCES += \
    resource.qrc
