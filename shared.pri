QT += core \
    quick \
    qml \
    gui \
    widgets
	
CONFIG += c++11 \
    resources_big

android {
    DEFINES += _ANDROID_PLATFORM
    ROUTDIR = bin\android
}

windows {
    DEFINES += _WINDOWS_PLATFORM
    ROUTDIR = bin\windows
}

DESTDIR = $$PWD/$$ROUTDIR
LIBS += -L$$PWD/$$ROUTDIR

INCLUDEPATH += \
        $$PWD/harfbuzz-ng/src \
        $$PWD/harfbuzz-ng/harfbuzz-ng-external
