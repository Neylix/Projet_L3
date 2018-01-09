#!/bin/sh
../arm_simulator --gdb-port 41300 &
pid=$!
arm-none-eabi-gdb $1 -x Test/test_normal.txt  > Test/Trace/trace_$1a.txt &
arm-none-eabi-gdb $1 -x Test/test_sim.txt > Test/Trace/trace_sim_$1a.txt &

wait $pid

tail -n +20 Test/Trace/trace_$1a.txt > Test/Trace/trace_$1.txt
tail -n +24 Test/Trace/trace_sim_$1a.txt > Test/Trace/trace_sim_$1.txt
rm Test/Trace/trace_$1a.txt Test/Trace/trace_sim_$1a.txt

diff -y Test/Trace/trace_sim_$1.txt Test/Trace/trace_$1.txt | grep \|
