QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET      = InspectMaster
TEMPLATE    = app

SOURCES += main.cpp


MOC_DIR     = temp/moc
RCC_DIR     = temp/rcc
UI_DIR      = temp/ui
OBJECTS_DIR = temp/obj


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


CONFIG += warn_off

CONFIG += resources_big

#CONFIG += console

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
include ($$PWD/map/map.pri)


OTHER_FILES +=  README.md \
                .gitignore  \
                LICENSE
