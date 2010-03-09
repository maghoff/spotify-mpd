#!/bin/bash

# Could be more precise, since we are only using the core and network qt modules:
PACKAGES="build-essential libqt4-dev curl libboost-dev libasound2-dev libao-dev"

function libspotify_is_installed {
    if [ -a /usr/local/libspotify.so ];
    then
        return 1
    else
        return 0
    fi
}

function install_libspotify {
    MACHINE=`uname -m` # Preferably i686 or x86_64
    SOURCE_URI="http://developer.spotify.com/download/libspotify/libspotify-0.0.3-linux6-$MACHINE.tar.gz"
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
