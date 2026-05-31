all:  
    g++  main.cpp   -o  bin/debug/doodle   -I inc  -L  lib   -lmingw32  -lSDL2main  -lSDL2   -lSDL2_image -lSDL2_ttf