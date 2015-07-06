#module load intel-mpi/5.0.1.035
#module load intel-compilers-15/15.0.0.090
#module load mpt/2.11

./clean.sh

make main

nMPI=$1
nOMP=$2

#qsub script.sh

export OMP_NUM_THREADS=$nOMP
export KMP_AFFINITY=disabled
export KMP_BLOCKTIME=0

mpiexec.mpich -np $nMPI ./main pascal > log.txt

#less log.txt
