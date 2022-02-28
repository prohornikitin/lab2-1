TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c99
LIBS += -lreadline -lcheck
INCLUDEPATH += include


SOURCES += \
        src/main.c \
        src/num_types.c \
        src/polynom.c \
        src/test_polynom.c \
        src/ui_base.c \
        src/ui_menu.c \
        src/vars.c

HEADERS += \
    include/errors_handle.h \
    include/num_types.h \
    include/polynom.h \
    include/test_polynom.h \
    include/test_polynom.h \
    include/ui.h \
    include/vars.h


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
