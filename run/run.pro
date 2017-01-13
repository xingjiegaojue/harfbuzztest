include( ../shared_app.pri )

INCLUDEPATH += ../freetype/inc \
TEMPLATE = app
TARGET = harfbuzztest

HEADERS += $$PWD/widget.h
SOURCES += $$PWD/widget.cpp \
           $$PWD/main.cpp
