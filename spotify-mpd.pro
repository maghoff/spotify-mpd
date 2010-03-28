# -------------------------------------------------
# Project created by QtCreator 2010-02-26T20:00:27
# -------------------------------------------------
QT += network \
    script \
    console
QT -= gui
TARGET = spotify-mpd
CONFIG += console
CONFIG -= app_bundle
CONFIG += link_pkgconfig
PKGCONFIG += libspotify \
    ao

# To find libspotify.so more easily at runtime:
unix:LIBS += -Wl,-R/usr/local/lib
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
    aoaudiooutput.cpp \
    spotifyplayer.cpp \
    spotifylink.cpp \
    spotifytrack.cpp \
    mpdserver.cpp \
    mpdlistener.cpp \
    scriptenvironment.cpp \
    scriptlistener.cpp \
    spotifyalbum.cpp
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
    aoaudiooutput.hpp \
    musicdeliverydata.hpp \
    spotifyplayer.hpp \
    spotifylink.hpp \
    spotifytrack.hpp \
    mpdserver.hpp \
    mpdlistener.hpp \
    scriptenvironment.hpp \
    qlog.hpp \
    scriptlistener.hpp \
    spotifyalbum.hpp \
    spotifyobjectwrapper.hpp
