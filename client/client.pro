#-------------------------------------------------
#
# Project created by QtCreator 2017-09-13T14:16:40
#
#-------------------------------------------------
include(../pedestrian.pri)
QT       += core gui network opengl
CONFIG +=qml c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pedestrian-client
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    client.cpp \
    videohandler.cpp \
    mainwidget.cpp

HEADERS += \
        mainwindow.h \
    client.h \
    common.h \
    config.h \
    protocol.h \
    yuvrender.h \
    camera.h \
    protocol.h \
    videohandler.h \
    videosrc.h \
    mainwidget.h

FORMS += \
        mainwindow.ui \
    form.ui

unix{
    DEFINES += IS_UNIX
    QMAKE_CXXFLAGS+="-std=c++11"
    INCLUDEPATH +="$$CVPATH/cv/opencv-249-linux32/include" \
    $$COMMON_INCLUDE_PATH
    LIBS+=-L$$CVPATH/cv/opencv-249-linux32/lib -lopencv_core -lopencv_highgui \
       -lopencv_objdetect -lopencv_imgproc -lopencv_ml -lopencv_video -lX11 \
    $$COMMON_LIB_PATH/libtools.a
    QMAKE_LIBS+=-lpthread
}
win32{
    INCLUDEPATH +="$$CVPATH/cv/opencv-vs2013-x86/include" \
    $$COMMON_INCLUDE_PATH
    LIBS+= -L$$COMMON_LIB_PATH/ -ltools

    CONFIG(debug, debug|release){
        LIBS+=-L$$CVPATH\cv\opencv-vs2013-x86\lib -lopencv_core249d -lopencv_highgui249d\
        -lopencv_objdetect249d -lopencv_imgproc249d -lopencv_ml249d -lopencv_video249d
    }else{
        LIBS+=-L$$CVPATH\cv\opencv-vs2013-x86\lib -lopencv_core249 -lopencv_highgui249 \
        -lopencv_objdetect249 -lopencv_imgproc249 -lopencv_ml249 -lopencv_video249
    }
}



DEFINES+=CLIENT
