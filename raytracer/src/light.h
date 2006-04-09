#ifndef INC_LIGHT_H_
#define INC_LIGHT_H_

/*-<==>-----------------------------------------------------------------
/ Basic point light with a single color 
/----------------------------------------------------------------------*/
class CLight {
  VECTOR loc;
  COLOR  color;
public:
  CLight (const VECTOR &aloc, const VECTOR &acolor) : loc (aloc), color(acolor) { }
  COLOR  getColor()    const { return color; }
  VECTOR getLocation() const { return loc;   }
};

#include <list>
typedef std::list<CLight *> LLights;

#endif
