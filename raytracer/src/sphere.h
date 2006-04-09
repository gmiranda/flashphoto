#ifndef INC_SPHERE_H_
#define INC_SPHERE_H_

#include "raytracer.h"

/*-<==>-----------------------------------------------------------------
/ Sphere of radius 'radius' 
/----------------------------------------------------------------------*/
class CSphere : public CRTObject {
  SCALAR radius;
public:
  CSphere(SCALAR aradius);
  bool hits (const CLine &line, SCALAR &hits);
  VECTOR getNormal (const VECTOR &loc);
};


#endif
