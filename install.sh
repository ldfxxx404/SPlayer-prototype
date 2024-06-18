#!/bin/bash 
path_to_binary=$1

cd src  
mkdir build
cd build
cmake ..
make 
chmod +x ConsoleMediaPlayer
cp ConsoleMediaPlayer $path_to_binary
 