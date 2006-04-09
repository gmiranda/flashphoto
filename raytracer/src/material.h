#ifndef INC_MATERIAL_H_
#define INC_MATERIAL_H_

#pragma warning (disable : 4786 )

/*-<==>-----------------------------------------------------------------
/ Generic Material definition
/ Some materials will not use the position to define the diffuse or 
/ reflectance values
/----------------------------------------------------------------------*/
class CMaterial {
public:
  CMaterial() {}
  virtual ~CMaterial() {}
  virtual COLOR  getDiffuseColor(const VECTOR &loc) const = 0;
  virtual SCALAR getReflectance(const VECTOR &loc) const = 0;
};

/*-<==>-----------------------------------------------------------------
/ A map, key is a std::string and the value is a material pointer
/----------------------------------------------------------------------*/
#include <map>
#include <string>
typedef std::map<std::string, CMaterial *> MMaterials;

/*-<==>-----------------------------------------------------------------
/ Solid color material
/----------------------------------------------------------------------*/
class CSolidMaterial : public CMaterial {
  COLOR  diffuse_color;
  SCALAR reflectance_factor;
public:
  CSolidMaterial(const COLOR &diffuse, SCALAR reflectance);
  COLOR  getDiffuseColor(const VECTOR &loc) const;
  SCALAR getReflectance(const VECTOR &loc)  const;
};

#endif
