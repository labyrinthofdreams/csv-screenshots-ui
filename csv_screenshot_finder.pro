#-------------------------------------------------
#
# Project created by QtCreator 2014-08-15T16:07:09
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = csv_screenshot_finder
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tablewidget.cpp \
    directorydialog.cpp

HEADERS  += mainwindow.hpp \
    tablewidget.hpp \
    directorydialog.hpp

FORMS    += mainwindow.ui \
    directorydialog.ui

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Wpedantic
