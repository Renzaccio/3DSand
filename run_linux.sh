#!/bin/sh

# This script is only used to define the path to the dynamic libraries for raylib
# Don't forget to "chmod +xrw ./run_linux.sh" to make it runnable

export LD_LIBRARY_PATH=./raylib/raylib-6.2_linux_amd64/lib
./main.bin