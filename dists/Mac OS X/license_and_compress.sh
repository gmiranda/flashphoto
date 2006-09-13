#!/bin/sh
# Adds the license agreement and compress the image
#
hdiutil convert -format UDCO flash_photo.dmg -o ../flash_photo4_exp_mac.dmg
hdiutil unflatten ../flash_photo4_exp_mac.dmg
/Developer/Tools/DeRez SLAResources > sla.r
/Developer/Tools/Rez -a sla.r -o ../flash_photo4_exp_mac.dmg
hdiutil flatten ../flash_photo4_exp_mac.dmg

