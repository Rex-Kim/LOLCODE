#!/bin/bash

cd vim_syntax
mkdir ~/.vim/ftdetect
cp lolcode-ftdetect.vim ~/.vim/ftdetect/lolcode.vim
mkdir ~/.vim/syntax
cp lolcode-syntax.vim ~/.vim/syntax/lolcode.vim

clear

printf "\n Syntax Installed\n\n"
