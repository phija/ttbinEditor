ttbinEditor
===========

graphical editor for fixing data (e.g. heartrate) inside ttbin files

System Requirements
===================

This program requires the following libraries to be compiled and installed
before attempting to build it.

1. `cmake`
   Available from https://cmake.org/
2. `curl`
   Available from http://curl.haxx.se/download.html
3. `qt5`
   Available from http://qt.io

Prebuilt packages should be available for most systems using the system's
built in package manager (dpkg, yum, apt, rpm etc...). Make sure that the
`-dev` version of the packages (eg. `libssl-dev`, `libcurl-dev`, `libusb-1.0-0-dev`)
are installed so that the headers are available.

Building the software
=====================

1. Go to the folder where you cloned the repository and create a new 
   subfolder called `build`.
2. Go into that new folder and run cmake by typing `cd build && cmake ..`. 
   If anything is missing or wrong cmake will tell you.
3. Now type `make` and the application should get compiled.
4. To run the application type `bin/ttbinEditor`.

Thanks
======

This project is only possible with the work of http://github.com/ryanbinns/ttwatch
Therefor my thanks go directly to him, thanks Ryan!

Realease Notes
==============

2016-11-6    Version 0.2
------------------------

* added average heartrate
* removed qDebugs output
* added consistent output to statusbar
* added entry to the list widget for heartrate (yes once there will be more)

2016-11-6    Version 0.1  
------------------------

* changing the heartrate with the mouse is now possbile
* loading and saving of ttbin files works via the menu


