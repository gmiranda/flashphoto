#include "sphere.h"

/*-<==>-----------------------------------------------------------------
/ Constructor
/----------------------------------------------------------------------*/
CSphere::CSphere(SCALAR aradius) {
  radius = aradius;
}

/*-<==>-----------------------------------------------------------------
/ 
/----------------------------------------------------------------------*/
bool CSphere::hits (const CLine &line, SCALAR &t_hit) {
	// ...
}

VECTOR CSphere::getNormal(const VECTOR &hit_loc) {
	// ...
}

