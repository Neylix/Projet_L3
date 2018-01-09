#!/bin/sh
../arm_simulator --gdb-port 41300 &
konsole -e "arm-none-eabi-gdb $1 -x test_file.txt"
