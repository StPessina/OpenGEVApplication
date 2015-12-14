#-------------------------------------------------
#
# Project created by QtCreator 2015-10-15T18:33:41
#
#-------------------------------------------------

QT       += core
QT       += network

QT       += gui

TARGET = OpenGEVTestApplication
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


HEADERS += \
    app.h \
    simpleviewer.h \
    depthobserver.h \
    rgbobserver.h  \
    depthrgbobserver.h

SOURCES += main.cpp \
    app.cpp \
    simpleviewer.cpp \
    depthobserver.cpp \
    rgbobserver.cpp  \
    depthrgbobserver.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build/build-opengev-qt4.8GCC-64bit-debug/release/ -lOpenGEV
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build/build-opengev-qt4.8GCC-64bit-debug/debug/ -lOpenGEV
else:unix {
    CONFIG(debug, debug|release): LIBS += -L$$PWD/../build/build-opengev-qt4.8GCC-64bit-debug -lOpenGEV
    CONFIG(release, release|debug): LIBS += -L$$PWD/../build/build-opengev-qt4.8GCC-64bit-release -lOpenGEV
}


INCLUDEPATH += $$PWD/../src
DEPENDPATH += $$PWD/../src

unix {
    INCLUDEPATH += /usr/include/pcl-1.7 \
                   /usr/include/eigen3 \
                   /usr/include/openni2 \
                   /usr/include/vtk-5.8 \
                   /usr/include/boost \
                   /usr/include/flann

    LIBS += "-L/usr/lib"

    LIBS += -L/usr/local/lib
    LIBS += -lboost_system \

    LIBS += -llog4cpp

    LIBS += -lpcl_common \
            -lpcl_visualization

    LIBS += -lvtkCommon \
            -lvtkGraphics \
            -lvtkFiltering \
            -lvtkIO \
            -lvtkImaging \
            -lvtkRendering \
            -lQVTK

    #LIBS += -lX11

    QMAKE_CXXFLAGS+= -std=c++11
}
