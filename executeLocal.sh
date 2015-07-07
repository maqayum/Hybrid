module load intel-mpi/5.0.1.035
module load intel-compilers-15/15.0.0.090
module load mpt/2.11

./clean.sh

export KMP_AFFINITY=scatter
export KMP_BLOCKTIME=0
export MPI_PROCESSES=$2
export OMP_NUM_THREADS=$3

make main
#omplace -nt $3
mpiexec -n $2  ./main $1 > log.txt

