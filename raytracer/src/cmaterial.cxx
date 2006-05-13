#include "raytracer.h"
#include "cmaterial.hxx"

bool CCheckerMaterial::is_white(const VECTOR &loc) const
{

  VECTOR dloc=loc;
  
  dloc.x=std::abs(dloc.x)+side_size/2;
  dloc.y=std::abs(dloc.y)+side_size/2;
  dloc.z=std::abs(dloc.z)+side_size/2;
  
  bool bx=(((int)(dloc.x/side_size)%2)==0);
  bool by=(((int)(dloc.y/side_size)%2)==0);
  bool bz=(((int)(dloc.z/side_size)%2)==0);
  
  if(bz)
    {
      if((!(bx&&by))&&(bx||by))
	return true;
      else 
	return false;
    }
  else
    {
      if((!(bx&&by))&&(bx||by))
	return false;
      else 
	return true;
    }
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

