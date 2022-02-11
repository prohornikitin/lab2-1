TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c99
LIBS += -lreadline

SOURCES += \
        main.c \
        polynom.c \
        ui.c

HEADERS += \
    errors_handle.h \
    polynom.h \
    ui.h
