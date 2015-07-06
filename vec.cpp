#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <iterator>
#include <cstdlib>
#include <sstream>
#include <fcntl.h>

#define _XOPEN_SOURCE 500
#include <unistd.h>

#include "vec.h"
#include "omp.h"

using namespace std;

template<typename T>
vector<T> split(const string& line) {
  istringstream is(line);
  return vector<T>(istream_iterator<T>(is), istream_iterator<T>());
}

////////////////////////////////////////////////////////////////////////////////
//CONSTRUCTEURS
VEC::VEC(int size){
  nR = size;
  X  = new double[nR];
  for(int i = 0 ; i < nR ; i++){
    X[i] = 0.0;
  }
}

VEC::VEC(const VEC& V){
  nR = V.nR;
  X  = new double[nR];
  for(int i = 0 ; i < nR ; i++){
    X[i] = V.X[i];
  }
}

VEC::VEC(string path,
	 string name){
  ifstream infile((path + "vector_" + name + "_H.data").c_str());
  string str;
  while(getline(infile, str)){
    vector<int> line = split<int>(str);
    nR = (int)line[0];
  }
  int Vf = open((path + "vector_" + name + "_V.bin").c_str(), O_RDONLY);
  X  = new double[nR];
  pread(Vf, X, nR*sizeof(*X), 0);
  close(Vf);
}

void VEC::del(){
  delete[] X;
}


////////////////////////////////////////////////////////////////////////////////
//OPERATEURS
VEC& VEC::operator=(const VEC& otherV){
  assert(nR == otherV.nR);
  for(int i=0;i<nR;i++){
    X[i] = otherV.X[i];
  }
  return *this;
}

VEC VEC::operator+() const{
  VEC V(nR);
  for(int i = 0 ; i < nR ; i++){
    V.X[i] = +X[i];
  }
  return V;
}
VEC VEC::operator+(const VEC& otherV) const{
  assert(nR == otherV.nR);
  VEC V(nR);
  for(int i = 0 ; i < nR ; i++){
    V.X[i] = otherV.X[i] + X[i];
  }
  return V;
}
void VEC::operator+=(const VEC& otherV){
  assert(nR == otherV.nR);
  for(int i = 0 ; i < nR ; i++){
    X[i] += otherV.X[i];
  }
}
VEC VEC::operator+(double a) const{
  VEC V(nR);
  for(int i = 0 ; i < nR ; i++){
    V.X[i] = X[i] + a;
  }
  return V;
}
void VEC::operator+=(double a){
  for(int i = 0 ; i < nR ; i++){
    X[i] += a;
  }
}

VEC VEC::operator-() const{
  VEC V(nR);
  for(int i = 0 ; i < nR ; i++){
    V.X[i] = -X[i];
  }
  return V;
}
VEC VEC::operator-(const VEC& otherV) const{
  assert(nR == otherV.nR);
  VEC V(nR);
  for(int i = 0 ; i < nR ; i++){
    V.X[i] = X[i] - otherV.X[i];
  }
  return V;
}
void VEC::operator-=(const VEC& otherV){
  assert(nR == otherV.nR);
  for(int i = 0 ; i < nR ; i++){
    X[i] -= otherV.X[i];
  }
}
VEC VEC::operator-(double a) const{
  VEC V(nR);
  for(int i = 0 ; i < nR ; i++){
    V.X[i] = X[i] - a;
  }
  return V;
}
void VEC::operator-=(double a){
  for(int i = 0 ; i < nR ; i++){
    X[i] -= a;
  }
}

VEC VEC::operator*(double a) const{
  VEC V(nR);
  for(int i = 0 ; i < nR ; i++){
    V.X[i] = X[i] * a;
  }
  return V;
}
void VEC::operator*=(double a){
  for(int i = 0 ; i < nR ; i++){
    X[i] *= a;
  }
}

VEC VEC::operator/(double a) const{
  VEC V(nR);
  for(int i = 0 ; i < nR ; i++){
    V.X[i] = X[i] / a;
  }
  return V;
}
void VEC::operator/=(double a){
  for(int i = 0 ; i < nR ; i++){
    X[i] /= a;
  }
}


int VEC::caracterize(string mess){
  cout << mess << ":" << endl;
  cout << "nR = "  << nR  << endl;
  cout << "V = "   <<
    X[0] << " "  << X[1] << "..." << X[nR-2]  << " " << X[nR-1]  << endl;
  cout << endl;
  return 0;
}
void VEC::write(string fileName) const{
  ofstream out;
  out.open(fileName.c_str());
  for(int i = 0 ; i < nR ; i++){
    out << X[i] << endl;
  }
  out.close();
}
