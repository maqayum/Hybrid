#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include <fcntl.h>
#include <iterator>
#include <algorithm>


#include <unistd.h>

#include "omp.h"
#include "mpi.h"

#include "vec.h"

using namespace std;

template<typename T>
vector<T> split(const string& line) {
  istringstream is(line);
  return vector<T>(istream_iterator<T>(is), istream_iterator<T>());
}

void debug(bool dbg, string str){
  MPI_Barrier(MPI_COMM_WORLD);
  if(dbg){
    cout << str << endl;
  }
}
void debug(bool dbg, int n){
  MPI_Barrier(MPI_COMM_WORLD);
  if(dbg){
    cout << n << endl;
  }
}

int main(int argc, char *argv[]){

  const string name(argv[1]);
  string path = "/work/norgeot/";

  //Ouverture
  MPI_Init(&argc, &argv);
  int mpiRank, mpiSize;
  MPI_Comm_rank(MPI_COMM_WORLD, &mpiRank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpiSize);
  bool DBG=(true)&&(mpiRank==0);

  int nThreads = 0;
#pragma omp parallel shared(nThreads)
    nThreads = omp_get_num_threads();

  if(DBG){
    cout << "Num threads   = " << nThreads << endl;
    cout << "Num MPI procs = " << mpiSize  << endl;
  }
  
  //Lecture du header de la matrice
  int nR = 0;
  int nnz = 0;
  if(mpiRank == 0){
    ifstream infile((path + "matrix_" + name + "_H.data").c_str());
    string str;
    while(getline(infile, str)){
      vector<int> line = split<int>(str);
      nR = (int)line[0];
      nnz = (int)line[1];
    }
  }
  MPI_Bcast(&nR, 1, MPI_INT, 0, MPI_COMM_WORLD);
  
  MPI_Barrier(MPI_COMM_WORLD);
  if(DBG){
    cout << "Num Rows      = " << nR  << endl;
    cout << "Num non zeros = " << nnz << endl;
  }

  //Création des chunks
  int mpiBegin = 0           + mpiRank * nR/mpiSize;
  int mpiEnd   = nR/mpiSize + mpiRank * nR/mpiSize;
  if(mpiRank == mpiSize-1){
    mpiEnd=nR;
  }
  int mpiChunk = mpiEnd - mpiBegin;
  
  //Ouverture des fichiers
  int Af  = open((path + "matrix_" + name + "_A.bin" ).c_str(), O_RDONLY);
  int IAf = open((path + "matrix_" + name + "_IA.bin").c_str(), O_RDONLY);
  int JAf = open((path + "matrix_" + name + "_JA.bin").c_str(), O_RDONLY);

  //Déclaration du runtime
  omp_set_schedule(omp_sched_auto, mpiChunk / nThreads);

  //Lecture de IA
  int *lIA = new int [mpiChunk + 1];
#pragma omp parallel for schedule(runtime)
  for(int  i = mpiBegin ; i < mpiEnd ; i++){
    lIA[i-mpiBegin]   = 0  ;
    lIA[i+1-mpiBegin] = 0;
  }
  pread(IAf, lIA, (mpiChunk+1) * sizeof(*lIA), mpiBegin * sizeof(*lIA));
  
  //Lecture de A et JA
  int nnzBegin = lIA[0];
  int nnzEnd = lIA[mpiChunk];
  int nnzChunk = nnzEnd - nnzBegin;
  double *lA  = new double [nnzChunk];
  int    *lJA = new int    [nnzChunk];
#pragma omp parallel for schedule(runtime)
  for(int  i = 0 ; i < mpiChunk ; i++){
    for(int j = lIA[i] ; j < lIA[i+1] ; j++){
      lA[j - nnzBegin ] = 0;
      lJA[j - nnzBegin] = 0;
    }
  }
  pread(JAf, lJA, (nnzChunk) * sizeof(*lJA), nnzBegin * sizeof(*lJA));
  pread(Af, lA, (nnzChunk) * sizeof(*lA), nnzBegin * sizeof(*lA));
  
  //Lecture des données
  VEC B(path, name);
  VEC R(B.nR);
  VEC X(B.nR);

  //Synchronization
  MPI_Barrier(MPI_COMM_WORLD);
  double t = omp_get_wtime();

  //Calcul parallèle
  double *SOL = new double[mpiChunk];
#pragma omp parallel for schedule(runtime)
  for(int  i = 0 ; i < mpiChunk ; i++){
    for(int j = lIA[i] ; j < lIA[i+1] ; j++){
      SOL[i] += lA[j-nnzBegin] * B.X[lJA[j-nnzBegin]];
    }
  }

  //Synchronization
  MPI_Barrier(MPI_COMM_WORLD);
  if(mpiRank==0){
    cout << omp_get_wtime() - t << endl;
    cout << SOL[0] << " " << SOL[1] << " ... ";
  }

  MPI_Barrier(MPI_COMM_WORLD);
  if(mpiRank==mpiSize-1){
    cout << SOL[mpiChunk-2] << " " << SOL[mpiChunk-1] << endl;
  }
  
  MPI_Finalize();

  delete[] SOL;
  delete[] lA;
  delete[] lIA;
  delete[] lJA;
}
