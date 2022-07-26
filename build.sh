#!/bin/sh

TARGET="fcc"
FLAGS="-std=c99 -Wextra -Wall -g"

gcc $FLAGS -o $TARGET main.c


