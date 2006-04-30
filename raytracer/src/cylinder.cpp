#include "cylinder.h"
#include <cmath>
/*-<==>-----------------------------------------------------------------
/ Constructor
/----------------------------------------------------------------------*/
/*CCylinder::CCylinder(SCALAR aheight, SCALAR aradius){
{
	height = aheight;
	radius = aradius;
}
*/
/*-<==>-----------------------------------------------------------------
/ Test a possible line hit
/----------------------------------------------------------------------*/
/*bool CCylinder::hits(const CLine &line, SCALAR &t_hit)
{
  VECTOR lloc=line.loc-loc;

  SCALAR a=line.dir.dot(lloc);
  SCALAR bp=pow((lloc.dot(line.dir)),2)-
    ((lloc.dot(lloc))-(radius*radius));

  // Si la raiz no tiene solucion, no hay interseccion
  if((bp)<0.0f)
    return false;

  SCALAR b=std::sqrt(bp);

//#define MOD(x) ((x<0)?(-(x)):(x))

  SCALAR t2=MOD(a)+b;

  if(t2<0)
    return false;

  SCALAR t1=MOD(a)-b;

  if(t1>0)
    t_hit=t1;
  else
    t_hit=t2;

  return true;
}

VECTOR CCylinder::getNormal(const VECTOR &hit_loc)
{
  VECTOR T;

  T=loc-hit_loc;

  T.normalize();
  return T;
}
*/
