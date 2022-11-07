QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
DEFINES += QT_DEPRECATED_WARNINGS
TEMPLATE = app
#CONFIG += console c++11
#CONFIG -= app_bundle
#CONFIG -= qt
TRANSLATIONS += brepop.ts
SOURCES += \
        main.cpp \
    ds.cpp \
    mainwindow.cpp \
    glwidget.cpp \
    BrepModel.cpp \
    matrix4d.cpp

HEADERS += \
    ds.h \
    mainwindow.h \
    glwidget.h \
    Entity.h \
    BrepModel.h \
    matrix4d.h

FORMS += \
    mainwindow.ui
