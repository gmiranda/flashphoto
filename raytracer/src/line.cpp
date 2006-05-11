#include <stdio.h>
#include "line.h"

/*-<==>-----------------------------------------------------------------
/ Builds a default empty line
/----------------------------------------------------------------------*/
CLine:: CLine() :  level(0), loc(0,0,0), dir (1,0,0),color(0,0,0), t(-1e6), obj(NULL) {
  }

/*-<==>-----------------------------------------------------------------
/ Builds a line with specific loc and direction
/----------------------------------------------------------------------*/
CLine::CLine (const VECTOR &nloc, const VECTOR &ndir, int nlevel)
	: level (nlevel), loc (nloc), dir (ndir) , color(0,0,0), t(-1e6), obj(NULL) {
  dir.normalize();
}

/*-<==>-----------------------------------------------------------------
/
/----------------------------------------------------------------------*/
CLine CLine::getReflected(const VECTOR &nloc, const VECTOR &normal)
{
  CLine c;
  dir.normalize();
  c.level=level+1;
  c.loc=nloc;
  c.dir=(dir+(2*dir.dot(-normal))*normal);
  c.dir.normalize();
  c.t=-1;

  return c;
}

const CLine& CLine::operator++()
{
  level++;
  return *this;
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
void CLine::addColor(const VECTOR &amount)
{
    // No sumamos luz negativa
    if(amount.x<0)
        return;
    if(amount.y<0)
        return;
    if(amount.z<0)
        return;
  color += amount;
  if(color.x>1)
    color.x=1;
  if(color.y>1)
    color.y=1;
  if(color.z>1)
    color.z=1;

  // NOTE: y esto para que?
  /*
    aixo era per si arrivaben a mes de 1 en un posar blanc del tot
  if((color.x>1)||(color.y>1)||(color.z>1))
    {
      color.x=1;
      color.y=1;
      color.z=1;
    }
  */
  
  //color.normalize();
}
