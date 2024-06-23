#!/bin/bash
PATH_TO_BINATY=$1

if [ -z "$PATH_TO_BINATY" ]; then
    echo "Введите путь до папки с медиа файлами"
else 
    cd src  
    mkdir -p build
    cd build
    cmake ..
    make 
    chmod +x ConsoleMediaPlayer
    cp ConsoleMediaPlayer "$PATH_TO_BINATY"
    rm -rf ConsoleMediaPlayer
fi
