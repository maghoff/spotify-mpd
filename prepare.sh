#!/bin/bash

if ! [ -a appkey.c ]
then
    echo "NOTE: You need to procure appkey.c from somewhere"
fi

# Could be more precise, since we are only using the core and network qt modules:
sudo apt-get install build-essential libqt4-dev
