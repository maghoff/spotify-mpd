#!/bin/bash

if ! [ -a appkey.c ]
then
    echo "NOTE: You need to procure appkey.c from somewhere"
fi

sudo apt-get install libqt4-dev libqt4-phonon-dev
