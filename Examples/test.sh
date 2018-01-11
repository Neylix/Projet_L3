#!/bin/sh
../arm_simulator --gdb-port 41300 &
term_pid=$(ps -o ppid= $PPID)
term_cmd=$(ps $term_pid | grep / | cut -d/ -f 2-)
/$term_cmd -e "arm-none-eabi-gdb $1 -x test_file.txt"
