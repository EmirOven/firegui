#! /bin/bash

g++ -std=c++17 -Wall -Wextra -o firegui main.cc $(pkg-config --cflags --libs sdl3-ttf)
