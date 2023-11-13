
contains(QMAKE_CC, cl) {
    INCLUDEPATH += $$PWD/msvc64/include \
                   $$PWD/msvc64/include/log4qt

    LIBS += -L$$PWD/msvc64/libs -llog4qt
}

HEADERS += \
    $$PWD/log.h
