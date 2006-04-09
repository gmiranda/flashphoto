#include "raytracer.h"

/*-<==>-----------------------------------------------------------------
/ 
/----------------------------------------------------------------------*/
CSolidMaterial::CSolidMaterial(const COLOR &diffuse, SCALAR reflectance) {
  diffuse_color = diffuse;
  reflectance_factor = reflectance;
}

/*-<==>-----------------------------------------------------------------
/ Diffuse and reflectance parameters are independent of the position
/----------------------------------------------------------------------*/
COLOR  CSolidMaterial::getDiffuseColor(const VECTOR &loc) const { 
  return diffuse_color; 
}

SCALAR CSolidMaterial::getReflectance(const VECTOR &loc)  const { 
  return reflectance_factor; 
}

