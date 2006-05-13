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
// TODO: Poder colocar el cilindro en otro punto diferente a (0,0,0).
bool CCylinder::hits(const CLine &line, SCALAR &t_hit)
{
  // Flags para saber donde hay interseccion
  bool lado=false, tapa=false;

  // La linea trasladada al origen
  CLine line_o(line.loc-loc,line.dir);

  // a * t^2 + b * t + c = 0
  // Esto queda...
  SCALAR a = line_o.dir.x*line_o.dir.x + line_o.dir.z*line_o.dir.z;

  SCALAR b = 2.0f*(line_o.loc.x*line_o.dir.x+line_o.loc.z*line_o.dir.z);
  SCALAR c = line_o.loc.x*line_o.loc.x+line_o.loc.z*line_o.loc.z-radius*radius;

  // Si a es 0, no se puede resolver
  if(a==0.0f)
    {
      std::cerr << "Cylinder::hits() a="<<a << std::endl;
      if(b==0.0f)return false;

    }
  // NOTE: Sacado de http://www.koders.com/cpp/fid5CAA2B18A838ACCA6922104409C271085D3B3F63.aspx?s=raytracer+cylinder
  // El discriminante es lo que va dentro de la raiz
  SCALAR discriminante = b*b / (4.0*a*a) - c/a;
  // Si es <0, no hay solucion
  if(discriminante<0.0f)
    {
      return false;
    }
  // Hay 2 posibles soluciones
  SCALAR rdisc = std::sqrt(discriminante);
  SCALAR t0 = -b/(2.0*a) - rdisc;
  SCALAR t1 = -b/(2.0*a) + rdisc;

  // EL punto de interseccion
  VECTOR i;

  // Me quedo con la solucion mas pequeña, positiva.
  if((t0>=0.0f)&&((t0<t1)||(t1<0.0f)))
    {
      t_hit=t0;
      //return true;
    }
  else if(t1>=0.0f){
    t_hit=t1;
    //return true;
  }

  // EL punto de interseccion
  i=line_o.loc+line_o.dir*t_hit;

  // Si no esta dentro del rango de altura esperado, no hay interseccion con lado
  if((i.y>=0.0f)&&(i.y<=height))
    {
      lado=true;
    }

  // Vamos a mirar las tapas
  SCALAR tTapa=0;

  // Si es posible la interseccion
  if(line_o.dir.y!=0.0f)
    {
      // Buscamos intersecciones con tapas
      //= -height-(line_o.loc.y+height*0)/line_o.dir.y;
      SCALAR tUp = -(line_o.loc.y-height)/line_o.dir.y;
      SCALAR tDown = (-line_o.loc.y)/line_o.dir.y;

      // Me quedo con la mas pequeña, positiva
      if((tUp>=0.0f)&&((tUp<tDown)||(tDown<0.0f)))
	{
	  // Quiero la de arriba
	  tTapa = tUp;
	  // Guardamos tipo de interseccion
	  intersection = UP;
	}
      else if(tDown>=0.0f)
	{
	  // La de abajo
	  tTapa = tDown;
	  // Guardamos tipo de interseccion
	  intersection = DOWN;
	}
      // Calculamos de nuevo el punto de interseccion
      i=line_o.loc+line_o.dir*tTapa;

      // Si no esta dentro del rango de esperado, no hay interseccion con tapa
      if((i.x*i.x+i.z*i.z)<=(radius*radius))
	{
	  // Hay colision con tapa
	  tapa = true;
	}
    }
  // Ahora miramos que interseccion es la mas cercana
  if(lado)
    {
      // Hay interseccion con el lado. Miramos si tambien con tapa
      if(tapa)
	{
	  // Comparamos: si la t del lado es mayor que la de la tapa
	  if(t_hit>tTapa)
	    // Nos quedamos con la de la tapa
	    t_hit = tTapa;
	  // Sino, pues tan panchos
	}
      // Si no ha habido, calculamos la normal del lado
      else
	{
	  // Guardamos el tipo de interseccion que ha habido
	  intersection = SIDE;
	}

      // Ha habido interseccion
      return (t_hit>1e-3);
    }
  // Si solo ha habido interseccion con tapa, ok
  if(tapa)
    {
      t_hit = tTapa;
      return (t_hit>1e-3);
    }
  // Si no hay interseccion con nada, a la mierda
  return false;
}

VECTOR CCylinder::getNormal(const VECTOR &hit_loc)
{
    switch(intersection){
        case UP:
            return VECTOR(0.0,1.0,0.0);
        case DOWN:
            return VECTOR(0.0,-1.0,0.0);
        case SIDE:
            return VECTOR(hit_loc.x,0,hit_loc.z).normalize();
    }

    // Si has llegado aqui vete a la mierda
	std::cerr << "CCylinder::getNormal() [ERROR] No se deberia haber llegado aqui"
                << std::endl;
    return VECTOR(0.0,0.0,0.0);
}

