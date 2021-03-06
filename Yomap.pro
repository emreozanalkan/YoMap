#-------------------------------------------------
#
# Project created by QtCreator 2013-12-23T16:41:55
#
#-------------------------------------------------

cache()

QT       += core gui opengl xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Yomap
TEMPLATE = app

CONFIG += warn_off precompile_header

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
    gui/mainwindow.cpp \
    db/poi/poicategory.cpp \
    db/poi/poipoint.cpp \
    gl/glpoipoint.cpp \
    db/Relations/path.cpp \
    db/Relations/pathsegment.cpp \
    gui/dialog.cpp \
    db/Relations/building.cpp

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
    db/db_definitions.hpp \
    db/poi/poicategory.h \
    db/poi/poipoint.h \
    gl/glpoipoint.h \
    db/Relations/path.h \
    db/Relations/pathsegment.h \
    gui/dialog.h \
    db/Relations/building.h

FORMS    +=gui/mainwindow.ui \
    gui/dialog.ui

macx{
PRECOMPILED_HEADER  = stable.h
LIBS += -framework GLUT
INCLUDEPATH += "/Users/emreozanalkan/boost_1_55_0"
LIBS += -L"/Users/emreozanalkan/boost_1_55_0/libs"
}
win32{
INCLUDEPATH += "C:\boost\boost_1_55_0\boost_1_55_0"
INCLUDEPATH += "C:\boost\boost_1_55_0"

INCLUDEPATH += "C:\MinGW\GLUT\include"
LIBS += -L"C:\MinGW\GLUT\lib"
LIBS += -lglut32 -lopengl32
}

RESOURCES += \
    data/settings.qrc \
    data/Resources.qrc

