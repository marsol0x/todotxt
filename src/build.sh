#!/bin/bash

mkdir -p ../bin

clang -std=c89 -O0 -g -o ../bin/todo main.c
clang -O0 -g -o ../bin/convert convert.c
