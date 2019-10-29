#!/bin/bash
cd $PBS_O_WORKDIR
#for n in 1 2 3 4; do
#echo $n >/dev/stderr
mpirun -np 8 a.out
#done
