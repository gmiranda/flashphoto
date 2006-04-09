#include <stdio.h>
#include "line.h"

/*-<==>-----------------------------------------------------------------
/ Builds a default empty line
/----------------------------------------------------------------------*/
CLine:: CLine() : loc(0,0,0), dir (1,0,0), color(0,0,0), level(0), t(-1e6), obj(NULL) {
  }

/*-<==>-----------------------------------------------------------------
/ Builds a line with specific loc and direction
/----------------------------------------------------------------------*/
CLine::CLine (const VECTOR &nloc, const VECTOR &ndir, int nlevel) : loc (nloc), dir (ndir), color(0,0,0), level (nlevel), t(-1e6), obj(NULL) {
  dir.normalize();
}

/*-<==>-----------------------------------------------------------------
/ 
/----------------------------------------------------------------------*/
CLine CLine::getReflected(const VECTOR &nloc, const VECTOR &normal) {
	// ..
}

/*-<==>-----------------------------------------------------------------
/ 
/----------------------------------------------------------------------*/
VECTOR CLine::getIntersection() const {
  return loc + (dir*t);
}

/*-<==>-----------------------------------------------------------------
/ return current recursion level 
/----------------------------------------------------------------------*/
int CLine::getLevel() const {
  return level;
}

/*-<==>-----------------------------------------------------------------
/ add a color amount to color of this line
/----------------------------------------------------------------------*/
void CLine::addColor(const VECTOR &amount) {
  color += amount;
	// ...
}

