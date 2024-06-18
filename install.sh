#!/bin/bash
path_to_binary=$1

if [ -z "$path_to_binary" ]; then
    echo "Введите путь до папки с медиа файлами"
else 
    cd src  
    mkdir -p build
    cd build
    cmake ..
    make 
    chmod +x ConsoleMediaPlayer
    cp ConsoleMediaPlayer "$path_to_binary"
    rm -rf ConsoleMediaPlayer
fi
