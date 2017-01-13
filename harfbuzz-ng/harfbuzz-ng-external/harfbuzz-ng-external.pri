INCLUDEPATH += $$PWD
HEADERS += $$PWD/hb-ft.h
SOURCES += $$PWD/hb-ft.cc

android {
SOURCES += $$PWD/errno.cc
}
