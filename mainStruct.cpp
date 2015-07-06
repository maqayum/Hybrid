#include <string>
#include <iostream>
#include "csr.h"
#include "vec.h"
#include "functions.h"

#include "omp.h"
#include "mkl.h"

using namespace std;

int main(int argc, char *argv[]){

  const string name(argv[1]);
  const string id(argv[2]);
  const int nP = atoi(argv[3]);  

  string path = "/work/norgeot/";

  VEC B(path, name, id); 
  VEC R(B.nR);
  VEC X(B.nR, id);

  omp_set_schedule(omp_sched_auto, 0.1 * B.nR/nP);
  omp_set_num_threads(nP);
  mkl_set_num_threads(nP);
  omp_set_nested(4);

  CSR A(path, name, nP, id);
  
  double t = omp_get_wtime();
  sequentialProduct(R, A, B);
  cout << "Temps séquentiel = " << omp_get_wtime() - t << endl;

  t = omp_get_wtime();
  parallelProduct(X, A, B, nP, id);
  cout << "Temps parallèle = " << omp_get_wtime() - t << endl;

  X.caracterize("Vecteur X");

  R.del(id);
  X.del(id);
  A.del(id);
  B.del(id);

  return 0;
}






