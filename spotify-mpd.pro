# -------------------------------------------------
# Project created by QtCreator 2010-02-26T20:00:27
# -------------------------------------------------
QT += network \
	script
QT -= gui

win32 {
	QT += console
}

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
    logger_base.cpp \
    log_message.cpp \
    console_logger.cpp \
    import/ao-audio.c \
    audiooutput.cpp \
    aoaudiooutput.cpp \
    scriptenvironment.cpp \
    scriptlistener.cpp \
    mpd/mpd_utils.cpp \
    mpd/mpdserver.cpp \
    mpd/mpdlistener.cpp \
	spotify/appkey.c \
	spotify/session.cpp \
    spotify/player.cpp \
    spotify/link.cpp \
    spotify/track.cpp \
    spotify/album.cpp \
    spotify/artist.cpp

HEADERS += application.hpp \
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
    scriptenvironment.hpp \
    qlog.hpp \
    scriptlistener.hpp \
    mpd/mpd_utils.hpp \
    mpd/mpdserver.hpp \
    mpd/mpdlistener.hpp \
    spotify/appkey.h \
    spotify/session.hpp \
    spotify/player.hpp \
    spotify/link.hpp \
    spotify/track.hpp \
    spotify/album.hpp \
    spotify/objectwrapper.hpp \
    spotify/artist.hpp

