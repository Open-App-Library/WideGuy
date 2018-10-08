#!/bin/sh
mkdir -p dist
rm -rf dist/*
cd dist

echo Creating ./dist/Makefile using qmake.....
qmake ../WideGuy.pro -o Makefile


echo Compiling application...
make

echo Moving over ../usr folder...
cp -r ../usr .
cp WideGuy usr/bin

echo Downloading LinuxDeployQt
wget https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage

echo Asking for permission to run command "chmod +x $PWD/linuxdeployqt-continuous-x86_64.AppImage"
pkexec chmod +x $PWD/linuxdeployqt-continuous-x86_64.AppImage

echo Creating AppImage...
./linuxdeployqt-continuous-x86_64.AppImage usr/share/applications/wideguy.desktop -appimage

echo
echo ===========================
echo DONE!!
echo AppImage is located in $PWD
