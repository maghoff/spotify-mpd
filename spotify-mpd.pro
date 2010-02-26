# -------------------------------------------------
# Project created by QtCreator 2010-02-26T20:00:27
# -------------------------------------------------
QT += network \
    phonon
QT -= gui
TARGET = spotify-mpd
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    application.cpp \
    mpd_utils.cpp
HEADERS += application.hpp \
    mpd_utils.hpp
