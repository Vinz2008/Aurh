#!/bin/bash
if [[ $1 = "-S" ]]; then
git clone https://aur.archlinux.org/$2.git /tmp/$2
cd /tmp/$2
makepkg -si
cd ~
rm -rf  /tmp/$2
else
echo 'Use a valid option'
fi
