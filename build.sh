#!/bin/bash

mkdir -p bin

clang -O0 -g -o bin/todo src/main.c
