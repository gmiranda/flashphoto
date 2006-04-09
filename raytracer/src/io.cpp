#include "raytracer.h"

std::ostream &operator << (std::ostream &os, const VECTOR &v) {
  os << v[0] << " " 
     << v[1] << " "
     << v[2] << " ";
  return os;
}

