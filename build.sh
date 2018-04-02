#!/bin/bash

# Custom build script by AidoP



# Compile source
libs="-ldl -lm -lX11 -lXext -lGLX -lGL -lGLU -lXrender"

gcc skypaper.c $libs -o build/skypaper

# Link objects


