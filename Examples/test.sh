#!/bin/sh
../arm_simulator --gdb-port 41300 &
atom $1.s
xfce4-terminal -e "arm-none-eabi-gdb $1 -x test_file.txt"
