include(../pedestrian.pri)
#message($$CVPATH) # execute 3 times

HEADERS+=\
    tools.h \
    camera.h \
    server.h
SOURCES+=main.cpp  \
    camera.cpp \
    server.cpp
QT+=network core
# CONFIG += qtc_runnable
VPATH=/root/repo-github/res/
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


#build_pass:CONFIG(debug, debug|release) {
#    unix: message("os:unix debug ")
#    win32: message("os:win32 debug")
#    else: message("os:else debug")
#}
#build_pass:CONFIG(release, debug|release) {
#    unix: message("os:unix release ")
#    win32: message("os:win32 release")
#    else: message("os:else release")
#}
#LIBS += -lX11 -L/root/sources/opencv-2.4.9/build/__install/lib/  -lopencv_core  -lopencv_highgui  -lopencv_objdetect -lopencv_imgproc -lopencv_ml -lopencv_video

#}
TARGET = pedestrian
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS




 #DEFINES += DISPLAY_VIDEO

