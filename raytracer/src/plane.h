#ifndef INC_PLANE_H_
#define INC_PLANE_H_

#include "raytracer.h"

/*-<==>-----------------------------------------------------------------
/ Plane defined by R*N = d
/----------------------------------------------------------------------*/
class CPlane : public CRTObject {
  VECTOR norm;
  SCALAR dist;
public:
  CPlane (const VECTOR &normal, SCALAR distance);
  bool hits (const CLine &line, SCALAR &t_hit);
  VECTOR getNormal (const VECTOR &loc);
};

#endif
