#!/bin/bash

# Makes it easier to test the program. Run using ./run.sh
# Needs to have execute permission => chmod u+x comp.sh

g++ main.cpp -o main
g++ scheduler.cpp -o scheduler

./scheduler &
./main &

wait