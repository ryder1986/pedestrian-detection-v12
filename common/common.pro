include(../pedestrian.pri)
HEADERS += \
    tools.h

SOURCES += \
    tools.cpp

TEMPLATE=lib
TARGET=tools

CONFIG += static

unix{
    QMAKE_CXXFLAGS+="--std=c++11"
#    target.path = /root/repo-github/res/common/lib-linux32
#    INSTALLS += target
}

head_install_setting.path=$$COMMON_INCLUDE_PATH
head_install_setting.files=tools.h
#  head_install_setting.commands=echo F | xcopy "$$absolute_path()/tools.h" $$COMMON_INCLUDE_PATH
lib_install_setting.path=$$COMMON_LIB_PATH
lib_install_setting.files=libtools.a
# lib_install_setting.commands=echo F | xcopy $$DESTDIR\tools.lib  $$COMMON_LIB_PATH
INSTALLS +=head_install_setting
INSTALLS +=lib_install_setting


#unix{
#   head_install_setting.path=$$COMMON_INCLUDE_PATH
#   head_install_setting.files=tools.h
#   head_install_setting.commands=cp $$PWD/tools.h $$COMMON_INCLUDE_PATH/
#   lib_install_setting.path=$$COMMON_LIB_PATH
#   lib_install_setting.files=libtools.a
#   lib_install_setting.commands=cp  libtools.a  $$COMMON_LIB_PATH
#   INSTALLS +=head_install_setting
#   INSTALLS +=lib_install_setting

#}
#win32{
#   head_install_setting.path=$$COMMON_INCLUDE_PATH
#   head_install_setting.files=tools.h
# #  head_install_setting.commands=echo F | xcopy "$$absolute_path()/tools.h" $$COMMON_INCLUDE_PATH
#   lib_install_setting.path=$$COMMON_LIB_PATH
#   lib_install_setting.files=libtools.a
#  # lib_install_setting.commands=echo F | xcopy $$DESTDIR\tools.lib  $$COMMON_LIB_PATH
#   INSTALLS +=head_install_setting
#   INSTALLS +=lib_install_setting

#}

#}
#win32{
#    target.path =$$COMMON_INCLUDE_PATH
#    INSTALLS += target
#}
QMAKE_APP_FLAG
#!exists( main.cpp ) {
#    error( "No main.cpp file found" )
#}
#message()
