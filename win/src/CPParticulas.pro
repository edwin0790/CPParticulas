#-------------------------------------------------
#
# Project created by QtCreator 2016-06-21T16:50:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CPParticulas
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    glwidget.cpp \
    window.cpp \
    particlewidget.cpp \
    particlelist.cpp

HEADERS  += \
    mainwindow.h \
    glwidget.h \
    window.h \
    particlewidget.h \
    errors.h \
    particlelist.h


DISTFILES +=

LIBS += -lopengl32 -lglu32

FORMS +=
