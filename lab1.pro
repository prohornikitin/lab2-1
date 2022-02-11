TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c99

SOURCES += \
        main.c \
        polynom.c

HEADERS += \
    fatal_error.h \
    polynom.h
