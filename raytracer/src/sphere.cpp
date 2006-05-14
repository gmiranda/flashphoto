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

  SCALAR t2=std::abs(a)+b;
  SCALAR t1=std::abs(a)-b;


  /*
  if((t1<0)&&(t2<0))
    return false;
  */

//caca
  if((t1>=1e-3)&&((t1<t2)||(t2<1e-3)))
  // caca
    t_hit=t1;
  else
    t_hit=t2;

  //t_hit=((std::abs(t2)<std::abs(t1))?t2:t1);

  return (t_hit>1e-3);
}

VECTOR CSphere::getNormal(const VECTOR &hit_loc)
{
  VECTOR T;

  T=hit_loc-loc;

  T.normalize();
  return T;
}

