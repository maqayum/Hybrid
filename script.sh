#!/bin/bash
#
#PBS -l walltime=04:00:00
#PBS -N main
#PBS -j oe 

#Chargement des modules
. /usr/share/modules/init/sh
module load intel-compilers-15/15.0.0.090
module load mpt/2.11
module load intel-mpi/5.0.1.035

cp $PBS_O_WORKDIR/main .

export KMP_AFFINITY=disabled
export KMP_BLOCKTIME=0
export MPI_PROCESSES=$nMPI
export OMP_NUM_THREADS=$nOMP

mpirun -n $nMPI omplace -nt $nOMP ./main $name > log_${name}_${nMPI}_${nOMP}.txt

cp *.txt $PBS_O_WORKDIR
