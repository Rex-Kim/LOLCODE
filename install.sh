#!/bin/bash

cd lolcode_compiler/
./compile.sh
sudo cp lolcode /usr/bin/lol
rm lolcode

clear

printf "\nLOLCODE compiler installed to /usr/bin/lol\n\nTo compile .lol file, run \"lol file.lol\"\n\n"
