#!/bin/sh

mkdir -p dist # Create dist directory. -p prevents error if already exist
rm -rf dist/* # Delete all in dist folder if anything is inside
cd dist       # cd into the dist.

# Run QMake in dist folder
echo
echo ========================================
echo Creating ./dist/Makefile using qmake....
echo ========================================
qmake ../WideGuy.pro -o Makefile || { echo 'Failed to run qmake. Is it in your path?' ; exit 1; }

# Compile the application
echo
echo ========================================
echo Compiling application...
echo ========================================
make || { echo 'Failed to compile project' ; exit 1; }

# Move the usr/ folder in dist/
echo 
echo ========================================
echo Moving over ../usr folder...
echo ========================================
cp -r ../usr .
mkdir usr/bin
cp WideGuy usr/bin

# Download LinuxDeployQt
echo 
echo ========================================
echo Downloading LinuxDeployQt...
echo ========================================
wget https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage

# Prompt the user for root password so we can make linuxdeployqt AppImage executable
echo 
echo ========================================
echo Asking for permission to make
echo LinuxDeployQt executable...
echo ========================================
pkexec chmod +x $PWD/linuxdeployqt-continuous-x86_64.AppImage

# Create the AppImage of our application
echo 
echo ========================================
echo Creating AppImage...
echo ========================================
./linuxdeployqt-continuous-x86_64.AppImage usr/share/applications/wideguy.desktop -appimage -extra-plugins=iconengines || { echo 'Failed building AppImage' ; exit 1; }

# If we made it this far, there are no errors. Print a success message.
echo
echo ========================================
echo Great success!!
echo AppImage is located in $PWD
echo ===========================

