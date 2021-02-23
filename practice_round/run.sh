#!/bin/bash

g++ -std=c++17 main.cpp -o main
for file in input/*.in
do
  ./main < $file
done