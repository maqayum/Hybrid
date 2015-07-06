#!/bin/bash
#
#PBS -l select=1:ncpus=16
#PBS -l walltime=04:00:00
#PBS -N main
#PBS -j oe 

#Chargement des modules
. /usr/share/modules/init/sh
module load intel-compilers-15/15.0.0.090
module load mpt/2.11
module load intel-mpi/5.0.1.035

#cp $PBS_O_WORKDIR/main .

export KMP_BLOCKTIME=0

export MPI_PROCESSES=2
export OMP_NUM_THREADS=1

mpiexec -n 1 omplace -nt $OMP_NUM_THREADS ./main > log.txt

#cp *.txt $PBS_O_WORKDIR
