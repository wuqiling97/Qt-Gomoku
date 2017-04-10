#-------------------------------------------------
#
# Project created by QtCreator 2016-08-31T16:09:07
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Gomoku
TEMPLATE = app


SOURCES += main.cpp\
        gomoku.cpp \
    createdialog.cpp \
    connectdialog.cpp \
    chessboard.cpp

HEADERS  += gomoku.h \
    createdialog.h \
    connectdialog.h \
    chessboard.h

FORMS    += gomoku.ui \
    createdialog.ui \
    connectdialog.ui

RESOURCES += \
    resource.qrc
