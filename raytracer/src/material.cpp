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
  //std::cout <<  loc << std::endl;
  return diffuse_color;
}

SCALAR CSolidMaterial::getReflectance(const VECTOR &loc)  const {
  return reflectance_factor;
}

SCALAR CSolidMaterial::getRefraction(const VECTOR &loc)  const {
  return 0.0;
}

/*-<==>-----------------------------------------------------------------
/ Transparent material
/----------------------------------------------------------------------*/
CTransparentMaterial::CTransparentMaterial(const COLOR &diffuse, SCALAR refraction){
	diffuse_color = diffuse;
	refraction_factor = refraction;
}

COLOR  CTransparentMaterial::getDiffuseColor(const VECTOR &loc) const {
  //std::cout <<  loc << std::endl;
  return diffuse_color;
}

SCALAR CTransparentMaterial::getReflectance(const VECTOR &loc)  const {
  return 0.0;
}

SCALAR CTransparentMaterial::getRefraction(const VECTOR &loc)  const {
  return refraction_factor;
}
