#-------------------------------------------------
#
# Project created by QtCreator 2017-09-11T16:35:19
#
#-------------------------------------------------

QT       += core gui
#network模块
QT += network
#xml QXmlQuery
QT += xmlpatterns
#DOM 方式处理 XML qt4 and qt5
QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ModbusDriver
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES +=main.cpp\
        mainwindow.cpp\
        xmlhelp.cpp

HEADERS +=mainwindow.h\
        xmlhelp.h

FORMS +=mainwindow.ui

DISTFILES += \
    config/configfile.xml

#qDebug() 类似于 cout，将后面的字符串打印到标准输出
QMAKE_CXXFLAGS += -std=c++0x
