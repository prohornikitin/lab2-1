TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c99
LIBS += -lreadline -lcheck
INCLUDEPATH += include


SOURCES += \
        src/main.c \
        src/polynom.c \
        src/test_polynom.c \
        src/ui_base.c \
        src/ui_menu.c

HEADERS += \
    include/errors_handle.h \
    include/polynom.h \
    include/test_polynom.h \
    include/test_polynom.h \
    include/ui.h


HEADERS += \
    include/src/errors_handle.h \
    include/src/polynom.h \
    include/src/test_polynom.h \
    include/src/ui.h


HEADERS += \
    include/src/errors_handle.h \
    include/src/polynom.h \
    include/src/test_polynom.h \
    include/src/ui.h


HEADERS += \
    include/src/errors_handle.h \
    include/src/polynom.h \
    include/src/test_polynom.h \
    include/src/ui.h
