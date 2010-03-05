# -------------------------------------------------
# Project created by QtCreator 2010-02-26T20:00:27
# -------------------------------------------------
QT += network
QT -= gui
TARGET = spotify-mpd
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    application.cpp \
    mpd_utils.cpp \
    spotify.cpp \
    spotifysession.cpp \
    appkey.c \
    logger_base.cpp \
    log_message.cpp \
    console_logger.cpp
HEADERS += application.hpp \
    mpd_utils.hpp \
    spotify.hpp \
    spotifysession.hpp \
    appkey.h \
    log.hpp \
    log_level.hpp \
    logger_base.hpp \
    log_message.hpp \
    console_logger.hpp
LIBS += -L/usr/local/lib \
    -Wl,-R/usr/local/lib \
    -lspotify
