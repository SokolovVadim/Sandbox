#!/bin/bash

#PBS -l walltime=00:01:00,nodes=7:ppn=4
#PBS -N 000
#PBS -q batch

cd $PBS_O_WORKDIR
mpirun --hostfile $PBS_NODEFILE -np 28 ./a.out ${ARG}
rm -rf a.out