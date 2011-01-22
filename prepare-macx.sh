#!/bin/bash

PACKAGES="libao boost"
LIBSPOTIFY_VERSION="0.0.6"


function libspotify_is_installed {
    if [ -d "/Library/Frameworks/libspotify.framework/Versions/$LIBSPOTIFY_VERSION" ]
    then
        return 0
    fi

    return 1
}

function install_libspotify {
    # Binary is universal across ppc, x86 and x86-64
    SOURCE_URI="http://developer.spotify.com/download/libspotify/libspotify-$LIBSPOTIFY_VERSION-Darwin.zip"
    TARGET=/tmp/spotify.tar.gz

    curl -o "$TARGET" "$SOURCE_URI"
    mkdir -p /tmp/spotify
    pushd /tmp/spotify

    unzip "$TARGET"
    cd libspotify*
    cp -r libspotify.framework /Library/Frameworks/

    popd
}


if ! [ -a spotify/appkey.c ]
then
    echo "NOTE: You need to procure spotify/appkey.c from somewhere"
fi

if ! which port >/dev/null
then
    echo "ERROR: You must manually install MacPorts"
    exit 1
fi

# Stupid port is not easily scriptable
# It offers "port installed" for checking, but it always evaluates to 0
sudo port install $PACKAGES


if ! libspotify_is_installed
then
    install_libspotify
fi


if ! which qmake >/dev/null
then
    echo "ERROR: You must manually install Qt"
    exit 1
fi

qmake

echo "NOTE: About Qt; In order to build 64-bit binaries, you must install the Cocoa-variant which is *not* in the default package"
echo "      See http://qt.nokia.com/downloads/qt-for-open-source-cpp-development-on-mac-os-x"
echo
echo "You should now be able to build with for example: xcodebuild -parallelizeTargets"
echo "Or you could use Qt Creator"
