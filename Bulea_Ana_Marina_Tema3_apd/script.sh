#!/bin/bash

module load libraries/openmpi-1.6-gcc-4.6.3
module load compilers/gnu-4.6.3

mpirun -np 4 ./tema3 topology2.txt 2/2-1.txt 2-1-out.txt
mpirun -np 4 ./tema3 topology2.txt 2/2-2.txt 2-2-out.txt
mpirun -np 4 ./tema3 topology2.txt 2/2-3.txt 2-3-out.txt

mpirun -np 9 ./tema3 topology3.txt 3/3-1.txt 3-1-out.txt
mpirun -np 9 ./tema3 topology3.txt 3/3-2.txt 3-2-out.txt
mpirun -np 9 ./tema3 topology3.txt 3/3-3.txt 3-3-out.txt
mpirun -np 9 ./tema3 topology3.txt 3/3-4.txt 3-4-out.txt

mpirun -np 16 ./tema3 topology4.txt 4/4-1.txt 4-1-out.txt

