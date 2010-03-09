# -------------------------------------------------
# Project created by QtCreator 2010-02-26T20:00:27
# -------------------------------------------------
QT += network
QT -= gui
TARGET = spotify-mpd
CONFIG += console
CONFIG -= app_bundle
CONFIG += link_pkgconfig
PKGCONFIG += libspotify \
    ao

# To find libspotify.so more easily at runtime:
linux {
    LIBS += -Wl,-R/usr/local/lib
}

TEMPLATE = app
SOURCES += main.cpp \
    application.cpp \
    mpd_utils.cpp \
    spotifysession.cpp \
    appkey.c \
    logger_base.cpp \
    log_message.cpp \
    console_logger.cpp \
    import/ao-audio.c \
    audiooutput.cpp \
    alsaaudiooutput.cpp \
    spotifyplayer.cpp \
    spotifylink.cpp \
    spotifytrack.cpp \
    mpdserver.cpp \
    mpdlistener.cpp
HEADERS += application.hpp \
    mpd_utils.hpp \
    spotifysession.hpp \
    appkey.h \
    log.hpp \
    log_level.hpp \
    logger_base.hpp \
    log_message.hpp \
    console_logger.hpp \
    import/queue.h \
    import/audio.h \
    audiooutput.hpp \
    alsaaudiooutput.hpp \
    musicdeliverydata.hpp \
    spotifyplayer.hpp \
    spotifylink.hpp \
    spotifytrack.hpp \
    mpdserver.hpp \
    mpdlistener.hpp
