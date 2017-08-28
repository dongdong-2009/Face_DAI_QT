#! /bin/sh

exe="camCapture"
des="/home/jack/build-camCapture-unknown-Release"
deslibs="/home/jack/build-camCapture-unknown-Release/Release"
delist=$(ldd $des/camCapture | awk '{if (match($3,"/")){ printf("%s "),$3 } }')
cp -v $deplist $deslibs
