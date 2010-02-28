#!/bin/bash

function libspotify_is_installed {
    return [ -a /usr/local/libspotify.so ]
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

# Could be more precise, since we are only using the core and network qt modules:
sudo apt-get install build-essential libqt4-dev curl

if ! libspotify_is_installed
then
    install_libspotify
fi

qmake-qt4
