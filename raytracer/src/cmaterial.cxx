#include "raytracer.h"
#include "cmaterial.hxx"

bool CCheckerMaterial::is_white(const VECTOR &loc) const
{
  
  bool bx;
  bool by;
  bool bz;
  
  return true;
}


CCheckerMaterial::CCheckerMaterial (CMaterial *white, CMaterial *black, SCALAR size)
{
  tile_white = white;
  tile_black = black;
  side_size=size;
}

COLOR CCheckerMaterial::getDiffuseColor(const VECTOR &loc) const
{
  if (is_white(loc))
    return tile_white->getDiffuseColor(loc);
  else
    return tile_black->getDiffuseColor(loc);
}

SCALAR  CCheckerMaterial::getReflectance(const VECTOR &loc) const
{
  if (is_white(loc))
    return tile_white->getReflectance(loc);
  else
    return tile_black->getReflectance(loc);
}

