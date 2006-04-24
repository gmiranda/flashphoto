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
    CLine lloc=line;

// TODO (#5#): Mirar si esto funciona siempre...
    lloc.loc=lloc.loc-this->loc;

    SCALAR w=lloc.loc.dot(lloc.dir);
    SCALAR c=(lloc.loc.dot(lloc.loc))-(radius*radius);

    // Si la raiz no tiene solucion, no hay interseccion
    if((w*w-c)<0.0f)
        return false;

    SCALAR q=std::sqrt(w*w-c);

    SCALAR t1=-w+q;
    SCALAR t2=-w-q;

    if(t1<t2)
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

