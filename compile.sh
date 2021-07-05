#! /bin/sh

g++ -std=c++11 main.cpp -o main -I include -F /Library/Frameworks -framework SDL2 -L . -l box2d