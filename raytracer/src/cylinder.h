#ifndef INC_CYLINDER_H_
#define INC_CYLINDER_H_

#include "raytracer.h"

/*-<==>-----------------------------------------------------------------
/ Cylinder of height 'height' and radius 'radius'
/----------------------------------------------------------------------*/
class CCylinder : public CRTObject {
  SCALAR radius;
  SCALAR height;
  //VECTOR norm;
  enum{SIDE=0,UP,DOWN} intersection;
public:
  CCylinder(SCALAR aheight, SCALAR aradius);
  bool hits (const CLine &line, SCALAR &hits);
  VECTOR getNormal (const VECTOR &loc);
};


#endif
