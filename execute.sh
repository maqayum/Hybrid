module load intel-mpi/5.0.1.035
module load intel-compilers-15/15.0.0.090
module load mpt/2.11

./clean.sh

make main

qsub -v name=$1,nMPI=$2,nOMP=$3 -l select=1:ncpus=$(($2*$3)) script.sh
