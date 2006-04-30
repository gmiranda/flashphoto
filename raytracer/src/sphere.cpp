#include "sphere.h"
#include <cmath>
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
  VECTOR lloc=line.loc-loc;

  SCALAR a=line.dir.dot(lloc);
  SCALAR bp=(lloc.dot(line.dir)*lloc.dot(line.dir))-
    ((lloc.dot(lloc))-(radius*radius));

  // Si la raiz no tiene solucion, no hay interseccion
  if((bp)<0.0f)
    return false;

  SCALAR b=std::sqrt(bp);

//#define MOD(x) ((x<0)?(-(x)):(x))
  // NOTE: No es lo mismo esto de aqui arriba que abs?
  SCALAR t2=std::abs(a)+b;

  if(t2<0)
    return false;

  SCALAR t1=std::abs(a)-b;

  if(t1>0)
    t_hit=t1;
  else
    t_hit=t2;

  return true;
}

VECTOR CSphere::getNormal(const VECTOR &hit_loc)
{
  VECTOR T;

  T=loc-hit_loc;

  T.normalize();
  return T;
}
