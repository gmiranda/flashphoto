#include "sphere.h"

/*-<==>-----------------------------------------------------------------
/ Constructor
/----------------------------------------------------------------------*/
CSphere::CSphere(SCALAR aradius)
{
  radius = aradius;
}

/*-<==>-----------------------------------------------------------------
/ 
/----------------------------------------------------------------------*/
bool CSphere::hits (const CLine &line, SCALAR &t_hit)
{
  return false;
}

VECTOR CSphere::getNormal(const VECTOR &hit_loc) 
{
  VECTOR T;
  
  T=loc-hit_loc;
  
  T.normalize();
  return T;
}

