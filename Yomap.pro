#-------------------------------------------------
#
# Project created by QtCreator 2013-12-23T16:41:55
#
#-------------------------------------------------

QT       += core gui opengl xml

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
    gui/mainwindow.cpp \
    db/poi/poicategory.cpp \
    db/poi/poipoint.cpp \
    gl/glpoipoint.cpp

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
    gl/glpoipoint.h

FORMS    +=gui/mainwindow.ui

macx{
LIBS += -framework GLUT
INCLUDEPATH += "/usr/local/Cellar/boost/1.55.0/include"
LIBS += -L"/usr/local/Cellar/boost/1.55.0/lib"
}
win32{
#INCLUDEPATH += "C:\boost\boost_1_55_0\boost_1_55_0"
INCLUDEPATH += "C:\boost\boost_1_55_0"

INCLUDEPATH += "C:\MinGW\GLUT\include"
LIBS += -L"C:\MinGW\GLUT\lib"
LIBS += -lglut32 -lopengl32
}

RESOURCES += \
    data/settings.qrc \
    data/Resources.qrc
