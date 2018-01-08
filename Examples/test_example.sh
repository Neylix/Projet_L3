#!/bin/sh
../arm_simulator --gdb-port 41300 &
atom $1.s
arm-none-eabi-gdb $1 -x Test/test_normal.txt  > Test/Trace/trace_$1.txt &
arm-none-eabi-gdb $1 -x Test/test_sim.txt > Test/Trace/trace_sim_$1.txt &

sleep 10

diff Test/Trace/trace_sim_$1.txt Test/Trace/trace_$1.txt
