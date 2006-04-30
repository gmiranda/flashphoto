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
	//std::cerr << "Cylinder::hits()" << std::endl;
	// Vector direcci�n (este cilindro estara siempre en el eje Y)
	//VECTOR dir(0,1,0);

	// a * t^2 + b * t + c = 0
	// Esto queda...
	SCALAR a = line.dir.x*line.dir.x + line.dir.z*line.dir.z;
	// Si a es 0, no se puede resolver
	if(a==0.0f){
		//std::cerr << "Cylinder::hits() a="<<a << std::endl;
		return false;
	}
	SCALAR b = 2*(line.loc.x*line.dir.x+line.loc.z*line.dir.z);
	// NOTE: este -1 sera luego -radio^2
	SCALAR c = line.loc.x*line.loc.x+line.loc.z*line.loc.z-radius*radius;

	// El discriminante es lo que va dentro de la iraz
	SCALAR discriminante = b*b-4*a*c;
	// Si es <0, no hay solucion
	if(discriminante<0.0f){
		//std::cerr << "Cylinder::hits() discriminante <0" << std::endl;
		return false;
	}
	// Hay 2 posibles soluciones
	// TODO: se puede simplificar (ver links.rtf)
	SCALAR t0 = (-b+std::sqrt(discriminante))/(2.0f*a);
	SCALAR t1 = (-b-std::sqrt(discriminante))/(2.0f*a);

	// Me quedo con la solucion mas peque�a, positiva.
	if((t0>0.0f)&&(t0<t1)){
		t_hit=t0;
		//return true;
	}
	else if(t1>0.0f){
		t_hit=t1;
		//return true;
	}

	// EL punto de interseccion
	VECTOR i;

	// Si es posible la interseccion
	if(line.dir.y!=0.0f){
		// Buscamos intersecciones con tapas
		SCALAR tUp = -(line.loc.y+height)/line.dir.y;
		SCALAR tDown = (line.loc.y)/line.dir.y;
		// Nota: por ahora t_hit es la interseccion con el lateral
//std::cerr << "tUp="<<tUp<<", tDown="<<tDown<<",tlateral="<<t_hit<<std::endl;
		// Buscamos t mas peque�a
		if((tUp<t_hit)&&(tUp>=0.0f)&&((tUp<tDown)||(tDown<0.0f))){
			// Miramos si esta dentro de la circunferencia de la tapa
			i=line.loc+line.dir*t_hit;
			std::cerr << "i="<<i<<std::endl;
			if(i.x*i.x+i.z*i.z<=radius*radius){
				std::cerr << "Cylinder::hits() me quedo con tUp" << std::endl;
				t_hit = tUp;
			}
		}
		else if((tDown<t_hit)&&(tDown>=0.0f)&&((tDown<tUp)||(tUp<0.0f))){
			// Miramos si esta dentro de la circunferencia de la tapa
			i=line.loc+line.dir*t_hit;
			std::cerr << "i="<<i<<std::endl;
			if(i.x*i.x+i.z*i.z<=radius*radius){
				std::cerr << "Cylinder::hits() me quedo con tDown" << std::endl;
				t_hit = tDown;
			}
		}
	}

	// EL punto de interseccion
	i=line.loc+line.dir*t_hit;

	// Si no esta dentro del rango de altura esperado, no hay interseccion
	if((i.y>=0.0f)&&(i.y<=height)){
		//std::cerr << "Cylinder::hits() Interseccion: " << i << std::endl;
		return true;
	}

	// Guay
	return false;
}

VECTOR CCylinder::getNormal(const VECTOR &hit_loc){
  VECTOR N;

  /*T=loc-hit_loc;

  T.normalize();*/

  // Si es por un lateral
  N=VECTOR(hit_loc.x,0,hit_loc.z);
  N.normalize();
  return N;
}

