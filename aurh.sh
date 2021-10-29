#!/bin/bash
if [[ $1 = "-S" ]]; then
git clone https://aur.archlinux.org/$2.git
cd $2
makepkg -si

else
echo 'Use a valid option'
fi
