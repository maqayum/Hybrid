#ifndef VECHEADERDEF
#define VECHEADERDEF

#include <string>


struct VEC{
  int nR;
  double *X;

  VEC(int size);
  VEC(const VEC& V);
  VEC(std::string path, std::string name);
  void del();
  
  VEC& operator=(const VEC& otherV);

  VEC operator+() const;
  VEC operator+(const VEC& otherV) const;
  void operator+=(const VEC& otherV);
  VEC operator+(double a) const;
  void operator+=(double a);

  VEC operator-() const;
  VEC operator-(const VEC& otherV) const;
  void operator-=(const VEC& otherV);
  VEC operator-(double a) const;
  void operator-=(double a);

  VEC operator*(double a) const;
  void operator*=(double a);

  VEC operator/(double a) const;
  void operator/=(double a);

  int caracterize(std::string mess);
  void write(std::string fileName) const;

};

#endif
