WideGuy
---

## A Linux tool that makes multi-monitor gaming easy. (A GUI tool for Xinerama)

If you are looking for documentation on using this application please [refer to the website](https://openapplibrary.org/project/wideguy).

This readme file is for developers who wish to compile this application

## Compiling

The easiest way is as follows:

- Download Qt5 and Qt Creator. I prefer using [the installer](https://www.qt.io/download) even on Linux distributions.
- Open WideGuy.pro in Qt Creator.
- Compile & run

## Building an AppImage

`sh make-appimage.sh`

*Note: It will ask for your root password in order to chmod the downloaded linuxdeployqt AppImage*
