#include "raytracer.h"
#include "cmaterial.hxx"

bool CCheckerMaterial::is_white(const VECTOR &loc) const
{
  
  VECTOR dloc=loc;
  
  if(dloc.x<0)
    {
      dloc.x=std::abs(dloc.x)+side_size/2;
    }
  if(dloc.y<0)
    {
      dloc.y=std::abs(dloc.x)+side_size/2;
    }
  if(dloc.z<0)
    {
      dloc.z=std::abs(dloc.x)+side_size/2;
    }
  
  bool bx=(((dloc.x)-((float)(int)(dloc.x/side_size))*side_size)> side_size/2);
  bool by=(((dloc.y)-((float)(int)(dloc.y/side_size))*side_size)> side_size/2);
  bool bz=(((dloc.z)-((float)(int)(dloc.z/side_size))*side_size)> side_size/2);
  
  if (bz)
    {
      if((!(bz&&by))&&(bx || by))
	return true;
      else 
	return false;
    }
  else
    {
      if(!((!(bx&&by)) && (bx||by)))
	return true;
      else 
	return false;
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

