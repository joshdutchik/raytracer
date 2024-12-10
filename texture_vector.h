#ifndef TEXTURE_VECTOR_H
#define TEXTURE_VECTOR_H

#include "utility.h"

class texture_vector
{
public:
  double e[6];

  texture_vector() : e{0, 0, 0, 0, 0, 0} {}
  texture_vector(double e5) : e{0, 0, 0, 0, 0, e5} {}
  texture_vector(double e0, double e1, double e2) : e{e0, e1, e2, 0, 0, 0} {}
  texture_vector(double e0, double e1, double e2, double e3, double e4) : e{e0, e1, e2, e3, e4, 0} {}
  texture_vector(double e0, double e1, double e2, double e3, double e4, double e5) : e{e0, e1, e2, e3, e4, e5} {}

  double red() const { return e[0]; }
  double green() const { return e[1]; }
  double blue() const { return e[2]; }
  double fuzz() const { return e[3]; }
  double refraction() const { return e[4]; }
  double texture() const { return e[5]; }

  double operator[](int i) const { return e[i]; }
  double &operator[](int i) { return e[i]; }
};

#endif