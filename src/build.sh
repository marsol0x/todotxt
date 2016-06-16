#!/bin/bash

mkdir -p ../bin

clang -O0 -g -o ../bin/todo main.c
clang -O0 -g -o ../bin/convert convert.c
