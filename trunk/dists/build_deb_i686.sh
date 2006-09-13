#!/bin/sh

chmod -R 755 deb_i686/DEBIAN/
cp -f ../bin/flash deb_i686/usr/local/bin
cp -f ../bin/FlashGUI deb_i686/usr/local/bin
dpkg -b deb_i686/ flash_photo4_exp_linux_iSSE.deb
