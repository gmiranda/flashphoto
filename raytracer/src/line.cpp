#include <stdio.h>
#include "line.h"
#include "raytracer.h"

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
  //c.level=level+1;
  c.loc=nloc;

  // Miramos si es reflexion difusa o perfecta
  // Es difusa si hemos hecho menos de 2 reflexiones y hay componente difusa
 /* if((level < 2)&& (obj->getMaterial().getDiffuseColor()!=VECTOR(0.0,0.0,0.0)){
		// calculate diffuse reflection
		vector3 N = prim->GetNormal( pi );
		vector3 RP = a_Ray.GetDirection() - 2.0f * DOT( a_Ray.GetDirection(), N ) * N;
		VECTOR RP = dir - 2.0 * dir.dot(normal)*normal;
		VECTOR RN1 = VECTOR( RP.z, RP.y, -RP.x);
		VECTOR RN2 = RP.cross(RN1);
		refl *= m_SScale;

  }
  else{*/
    //VECTOR R = dir -2.0f * dir.dot(normal)*normal;
    c.dir = dir -2.0f * dir.dot(normal)*normal;
  //}
  //c.dir=dir+(2*(-normal.dot(dir))*normal);
  c.dir.normalize();
  c.t=-1;
  c.level = level+1;

  return c;
}

CLine CLine::getRefracted(const VECTOR &nloc, const VECTOR &normal, const SCALAR& n)
{
  dir.normalize();

  // El coseno de theta i es el escalar de la normal por el vector incidente
  const SCALAR cosi = -dir.dot(normal);
  // El coseno de theta t al cuadrado
  const SCALAR cos2t = 1.0f - n*n * (1.0f - cosi*cosi);

  assert(cos2t>0.0);

  // El vector refractado
  VECTOR dirRefr = (n * dir) + (n * cosi - std::sqrt( cos2t )) * normal;
  dirRefr.normalize();
  // Construimos la linea del haz reflejado
  CLine refr(nloc, dirRefr);
  refr.level=level+1;

  // Y ponemos el color a 0
  refr.color=COLOR(0.0,0.0,0.0);

  return refr;
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
