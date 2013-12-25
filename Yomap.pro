#-------------------------------------------------
#
# Project created by QtCreator 2013-12-23T16:41:55
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Yomap
TEMPLATE = app


SOURCES += main.cpp\
    algorithms/pathalgorithms.cpp \
    db/node.cpp \
    db/database.cpp \
    db/Relations/waysegment.cpp \
    db/Relations/way.cpp \
    db/Relations/relation.cpp \
    gl/glwidget.cpp \
    gl/glcamera.cpp \
    gui/logic.cpp \
    gui/mainwindow.cpp

HEADERS  += algorithms/pathalgorithms.h \
    db/node.h \
    db/database.h \
    db/CommonFunctions.hpp \
    db/Relations/waysegment.h \
    db/Relations/way.h \
    db/Relations/relation.h \
    gl/glwidget.h \
    gl/glcamera.h \
    stable.h \
    gui/logic.h \
    gui/mainwindow.h \
    db/db_definitions.hpp

FORMS    +=gui/mainwindow.ui

INCLUDEPATH += "C:\boost\boost_1_55_0\boost_1_55_0"

INCLUDEPATH += "C:\MinGW\GLUT\include"
LIBS += -L"C:\MinGW\GLUT\lib"
LIBS += -lglut32 -lopengl32
