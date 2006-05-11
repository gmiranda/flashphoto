#ifndef _RAY_CMATERIAL_H_
#define _RAY_CMATERIAL_H_

class CCheckerMaterial : public CMaterial 
{
  CMaterial *tile_white;
  CMaterial *tile_black;
  SCALAR side_size;
  
  bool is_white(const VECTOR &loc) const;
public:
  CCheckerMaterial (CMaterial *white, CMaterial *black, SCALAR size);
  COLOR getDiffuseColor(const VECTOR &loc) const;
  SCALAR getReflectance(const VECTOR &loc) const;
};

#endif
