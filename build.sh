#!/bin/bash

# Custom build script by AidoP



# Compile source
libs="-ldl -lpthread -lm -lX11 -lXext -lGLX -lGL -lGLU -lXrender"

#gcc glad/src/glad.c -I glad/include -ldl -o glad.o

gcc skypaper.c glad/src/glad.c $libs -o build/skypaper -I glad/include
