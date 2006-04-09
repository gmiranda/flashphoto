#include "plane.h"

/*-<==>-----------------------------------------------------------------
/ n.x * x + n.y * y + n.z * z = d
/----------------------------------------------------------------------*/
CPlane::CPlane (const VECTOR &normal, SCALAR distance) : norm(normal), dist(distance) {
  // ..
}

bool CPlane::hits (const CLine &line, SCALAR &t_hit) {
	// ..
}

VECTOR CPlane::getNormal (const VECTOR &loc) {
  // ..
}

