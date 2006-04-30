#include "plane.h"

/*-<==>-----------------------------------------------------------------
  / n.x * x + n.y * y + n.z * z = d
  /----------------------------------------------------------------------*/
CPlane::CPlane (const VECTOR &normal, SCALAR distance) : norm(normal), dist(distance) {
  // ..
}

bool CPlane::hits (const CLine &line, SCALAR &t_hit)
{
  SCALAR numerador,denominador;
  denominador = line.dir.dot(norm);

  // Si el denominador es 0, hacemos trampa
  if(denominador == 0.0f)
    denominador = 1e-5;

  //perque -dist? no és dist?
  numerador = -dist - norm.dot(line.loc);
  t_hit=numerador/denominador;
  //std::cerr << "t="<<t_hit<<std::endl;


  return (t_hit>0.0f);
}

VECTOR CPlane::getNormal (const VECTOR &loc)
{
  // En el caso del plano la normal es siempre la misma
  return norm;
}

