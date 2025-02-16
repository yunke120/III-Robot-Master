QT += network
greaterThan(QT_MAJOR_VERSION, 4) {
lessThan(QT_MAJOR_VERSION, 6) {
android {QT += androidextras}
} else {
QT += core-private
}}


MOC_DIR     = temp/moc
RCC_DIR     = temp/rcc
UI_DIR      = temp/ui
OBJECTS_DIR = temp/obj


!android {
!wasm {
message("Current compiler: $$QMAKE_CC")
contains(QMAKE_CC, cl) {
message("Current compiler: MSVC")
    DESTDIR = $$PWD/../msvc-build
}

contains(QMAKE_CC, gcc) {
message("Current compiler: MinGW")
    DESTDIR = $$PWD/../mingw-build
}}}


CONFIG += warn_off

CONFIG += resources_big

#CONFIG += console

#CONFIG -= debug_and_release


include ($$PWD/h3.pri)

INCLUDEPATH += $$PWD

HEADERS += $$PWD/appdata.h
SOURCES += $$PWD/appdata.cpp

HEADERS += $$PWD/appinit.h
SOURCES += $$PWD/appinit.cpp

HEADERS += $$PWD/base64helper.h
SOURCES += $$PWD/base64helper.cpp

HEADERS += $$PWD/customstyle.h
SOURCES += $$PWD/customstyle.cpp

HEADERS += $$PWD/iconhelper.h
SOURCES += $$PWD/iconhelper.cpp

HEADERS += $$PWD/quihelper.h
SOURCES += $$PWD/quihelper.cpp


!contains(DEFINES, no_qrc_image) {
RESOURCES += $$PWD/qrc/image.qrc
}

!contains(DEFINES, no_qrc_qm) {
RESOURCES += $$PWD/qrc/qm.qrc
}

!contains(DEFINES, no_qrc_font) {
RESOURCES += $$PWD/qrc/font.qrc
}
