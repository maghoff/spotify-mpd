#!/bin/bash

# Could be more precise, since we are only using the core and network qt modules:
PACKAGES="build-essential pkg-config libqt4-dev curl libboost-dev libasound2-dev libao-dev"

LIBSPOTIFY_VERSION="0.0.4"

function libspotify_is_installed {
    VSN=`pkg-config --modversion libspotify 2> /dev/null`

    if [ "$VSN" = "$LIBSPOTIFY_VERSION" ]
    then
        return 0
    fi

    return 1
}

function install_libspotify {
    MACHINE=`uname -m` # Preferably i686 or x86_64
    SOURCE_URI="http://developer.spotify.com/download/libspotify/libspotify-$LIBSPOTIFY_VERSION-linux6-$MACHINE.tar.gz"
    TARGET=/tmp/spotify.tar.gz

    curl -o "$TARGET" "$SOURCE_URI"
    mkdir -p /tmp/spotify
    pushd /tmp/spotify

    tar zxf "$TARGET"
    cd libspotify*
    sudo make prefix=/usr/local install

    popd
}


if ! [ -a appkey.c ]
then
    echo "NOTE: You need to procure appkey.c from somewhere"
fi

if ! dpkg -l $PACKAGES >/dev/null
then
    echo "Will install packages: $PACKAGES"
    sudo apt-get install $PACKAGES
fi

if ! libspotify_is_installed
then
    install_libspotify
fi

qmake-qt4

