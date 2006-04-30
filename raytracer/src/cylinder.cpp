#include "cylinder.h"
#include <cmath>
/*-<==>-----------------------------------------------------------------
/ Constructor
/----------------------------------------------------------------------*/
CCylinder::CCylinder(SCALAR aheight, SCALAR aradius){
	height = aheight;
	radius = aradius;
}

/*-<==>-----------------------------------------------------------------
/ Test a possible line hit
/----------------------------------------------------------------------*/
bool CCylinder::hits(const CLine &line, SCALAR &t_hit){
	// Vector dirección (este cilindro estara siempre en el eje Y)
	VECTOR dir(0,1,0);

	// a * t^2 + b * t + c = 0
	// Esto queda...
	SCALAR a = dir.x*dir.x + dir.z*dir.z;
	// Si a es 0, no se puede resolver
	if(a==0.0f)
		return false;
	SCALAR b = 2*(loc.x*dir.x+loc.z*dir.z);
	// NOTE: este -1 sera luego -radio^2
	SCALAR c = loc.x*loc.x+loc.z*loc.z-1;

	// El discriminante es lo que va dentro de la iraz
	SCALAR discriminante = b*b-4*a*c;
	// Si es <0, no hay solucion
	if(discriminante<0.0f)
		return false;
	// Hay 2 posibles soluciones
	// TODO: se puede simplificar (ver links.rtf)
	SCALAR t0 = (-b+std::sqrt(discriminante))/(2.0f*a);
	SCALAR t1 = (-b-std::sqrt(discriminante))/(2.0f*a);

	// Me quedo con la solucion mas pequeña, positiva.
	if((t0>0.0f)&&(t0<t1)){
		t_hit=t0;
		return true;
	}
	else if(t1>0.0f){
		t_hit=t1;
		return true;
	}
	// NO deberiamos llegar nunca hasta aqui...
	std::cerr << "CCylinder::hits() [ERROR] esto no deberia ocurrir" << std::endl;
	return false;
}

VECTOR CCylinder::getNormal(const VECTOR &hit_loc){
  VECTOR T;

  /*T=loc-hit_loc;

  T.normalize();*/

  // Si es por un lateral
  T=VECTOR(hit_loc.x,0,hit_loc.z);
  T.normalize();
  return T;
}

