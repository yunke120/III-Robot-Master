QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET      = InspectMaster
TEMPLATE    = app

SOURCES += main.cpp

#指定编译产生的文件分门别类放到对应目录
MOC_DIR     = temp/moc
RCC_DIR     = temp/rcc
UI_DIR      = temp/ui
OBJECTS_DIR = temp/obj

#指定编译生成的可执行文件放到源码上一级目录下的bin目录
!android {
!wasm {
message("Current compiler: $$QMAKE_CC")
contains(QMAKE_CC, cl) {
message("Current compiler: MSVC")
    DESTDIR = $$PWD/msvc-build
}

contains(QMAKE_CC, gcc) {
message("Current compiler: MinGW")
    DESTDIR = $$PWD/mingw-build
}}}

#把所有警告都关掉
CONFIG += warn_off
#开启大资源支持
CONFIG += resources_big
#开启后会将打印信息用控制台输出
#CONFIG += console
#开启后不会生成空的 debug release 目录
#CONFIG -= debug_and_release



RESOURCES   += $$PWD/core_qss/qss.qrc
RESOURCES   += $$PWD/other/main.qrc

include ($$PWD/core_base/core_base.pri)
include ($$PWD/form/form.pri)
include ($$PWD/rockerbutton/rockerbutton.pri)
include ($$PWD/dashboard/dashboard.pri)
#include ($$PWD/battery/xbattery.pri)
include ($$PWD/statuswidget/statuswidget.pri)
include ($$PWD/log4qt/log4qt.pri)
include ($$PWD/sql/sql.pri)
include ($$PWD/video/video.pri)
include ($$PWD/protocol/protocol.pri)

OTHER_FILES +=  README.md \
                .gitignore  \
                LICENSE
