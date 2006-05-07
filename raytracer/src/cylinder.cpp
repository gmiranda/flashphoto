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
/*bool CCylinder::hits(const CLine &line, SCALAR &t_hit){

	// EL punto de interseccion
	VECTOR i;

	// Si es posible la interseccion
	if(line.dir.y!=0.0f){
		// Buscamos intersecciones con tapas
		//= -height-(line.loc.y+height*0)/line.dir.y;
		SCALAR tUp = -(line.loc.y-height)/line.dir.y;
		SCALAR tDown = (-line.loc.y)/line.dir.y;
		// Nota: por ahora t_hit es la interseccion con el lateral

		// Buscamos t mas pequeña
		if((tUp>=0.0f)&&((tUp<tDown)||(tDown<0.0f))){
			// Miramos si esta dentro de la circunferencia de la tapa
			i=line.loc+line.dir*tUp;
			//std::cerr << "tUp="<<tUp<<", tDown="<<tDown<<",tlateral="<<t_hit<<std::endl;
			//std::cerr << "i="<<i<<std::endl;
			if(i.x*i.x+i.z*i.z<=radius*radius){
				//std::cerr << "tUp="<<tUp<<", tDown="<<tDown<<",tlateral="<<t_hit<<std::endl;
				std::cerr << "Cylinder::hits() me quedo con tUp" << std::endl;
				t_hit = tUp;
			}
		}
		else if((tDown>=0.0f)&&((tDown<tUp)||(tUp<0.0f))){
			// Miramos si esta dentro de la circunferencia de la tapa
			i=line.loc+line.dir*tDown;
			//std::cerr << "i="<<i<<std::endl;
			if(i.x*i.x+i.z*i.z<=radius*radius){
				//std::cerr << "tUp="<<tUp<<", tDown="<<tDown<<",tlateral="<<t_hit<<std::endl;
				std::cerr << "Cylinder::hits() me quedo con tDown" << std::endl;
				t_hit = tDown;
			}
		}
	}
	//return true;
	// EL punto de interseccion
	i=line.loc+line.dir*t_hit;
//std::cerr << "interseccion.y=" << i.y << std::endl;
	// Si no esta dentro del rango de altura esperado, no hay interseccion
	if((i.y>=0.0f)&&(i.y<=height)){
		//std::cerr << "Cylinder::hits() Interseccion: " << i << std::endl;
		return true;
	}
	else
		//std::cerr << "Cylinder::hits() No hay  interseccion en: " << i << std::endl;

	// Guay
	return false;
}*/
bool CCylinder::hits(const CLine &line, SCALAR &t_hit){
	// Flags para saber donde hay interseccion
	bool lado=false, tapa=false;
	//std::cerr << "Cylinder::hits()" << std::endl;
	// Vector dirección (este cilindro estara siempre en el eje Y)
	//VECTOR dir(0,1,0);

	// a * t^2 + b * t + c = 0
	// Esto queda...
	SCALAR a = line.dir.x*line.dir.x + line.dir.z*line.dir.z;

	SCALAR b = 2.0f*(line.loc.x*line.dir.x+line.loc.z*line.dir.z);
	// NOTE: este -1 sera luego -radio^2
	SCALAR c = line.loc.x*line.loc.x+line.loc.z*line.loc.z-radius*radius;

	// Si a es 0, no se puede resolver
	if(a==0.0f){
		std::cerr << "Cylinder::hits() a="<<a << std::endl;
		if(b==0.0f)return false;

	}

	// El discriminante es lo que va dentro de la raiz
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

	// EL punto de interseccion
	VECTOR i;

	// Me quedo con la solucion mas pequeña, positiva.
	if((t0>=0.0f)&&((t0<t1)||(t1<0.0f))){
		t_hit=t0;
		//return true;
	}
	else if(t1>=0.0f){
		t_hit=t1;
		//return true;
	}

	// EL punto de interseccion
	i=line.loc+line.dir*t_hit;

	// Si no esta dentro del rango de altura esperado, no hay interseccion con lado
	if((i.y>=0.0f)&&(i.y<=height)){
		//std::cerr << "Cylinder::hits() Interseccion: " << i << std::endl;
		lado=true;
	}

	// Vamos a mirar las tapas
	SCALAR tTapa;
	// Si es posible la interseccion
	if(line.dir.y!=0.0f){
		// Buscamos intersecciones con tapas
		//= -height-(line.loc.y+height*0)/line.dir.y;
		SCALAR tUp = -(line.loc.y-height)/line.dir.y;
		SCALAR tDown = (-line.loc.y)/line.dir.y;

		// Me quedo con la mas pequeña, positiva
		if((tUp>=0.0f)&&((tUp<tDown)||(tDown<0.0f))){
			// Quiero la de arriba
			tTapa = tUp;
		}
		else if(tDown>=0.0f){
			// La de abajo
			tTapa = tDown;
		}
		// Calculamos de nuevo el punto de interseccion
		i=line.loc+line.dir*tTapa;

		// Si no esta dentro del rango de esperado, no hay interseccion con tapa
		if((i.x*i.x+i.z*i.z)<=(radius*radius)){
			//std::cerr << "Cylinder::hits() Interseccion: " << i << std::endl;
			// Hay colision con tapa
			tapa = true;
		}
	}
	// Ahora miramos que interseccion es la mas cercana
	if(lado){
		// Hay interseccion con el lado. Miramos si tambien con tapa
		if(tapa){
			// Comparamos: si la t del lado es mayor que la de la tapa
			if(t_hit>tTapa)
				// Nos quedamos con la de la tapa
				t_hit = tTapa;
			// Sino, pues tan panchos
		}
		// Ha habido interseccion
		return true;
	}
	// Si solo ha habido interseccion con tapa, ok
	if(tapa){
		t_hit = tTapa;
		return true;
	}
	// Si no hay interseccion con nada, a la mierda
	return false;

}

VECTOR CCylinder::getNormal(const VECTOR &hit_loc){
  VECTOR N;

  /*T=loc-hit_loc;

  T.normalize();*/

  // Si es por un lateral
  /*N=VECTOR(hit_loc.x,0,hit_loc.z);
  N.normalize();*/
  // Tapa superior
  N=VECTOR(0,1,0);
  // Tapa inferior
  //N= VECTOR(0,-1,0);
  return N;
}

