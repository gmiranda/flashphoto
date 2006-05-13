#ifndef _ESTATS_RAY_TRACER_HXX_
#define _ESTATS_RAY_TRACER_HXX_

#include "time.h"

class Estats
{
protected:
  static Estats inst;
  Estats(){}
  
  /*
    segons enunciat
    ===============
    * Número total de lineas trazadas
    * Número lineas trazadas desde la cámara
    * Número lineas trazadas para el cálculo de sombras
    * Número lineas trazadas para el cálculo de reflejos
    * Número lineas trazadas para el cálculo de refracción y reflejos
    * Número total de tests de intersección hechos
    * Número de tests de intersección con resultado positivo y el porcentaje respecto al total.
   */
  int lineas;
  int camera;
  int sombra;
  int reflexe;
  int refrac;
  int intersects;
  int interok;
  int interbad;
  time_t tempsInici, tempsFi;
public:
  static Estats& getInstance(){return inst;}
  void incLine(){lineas++;}
  void incCamera(){camera++;}
  void incSombra(){sombra++;}
  void incReflexe(){reflexe++;}
  void incRefrac(){refrac++;}
  void incIntersects(){intersects++;}
  void incIntersectsOK(){interok++;}
  void incIntersectsNO(){interbad++;}
  
  void start(){tempsInici=time(NULL);}
  void stop(){tempsFi=time(NULL);}
  
  //
  void printStats(CRayTracer &rt)
  {
    std::cout << "* Número total de lineas trazadas: " 
	      << lineas
	      << std::endl;
    std::cout << "* Número lineas trazadas desde la cámara: " 
	      << camera
	      << std::endl;
    std::cout << "* Número lineas trazadas para el cálculo de sombras: " 
	      << sombra
	      << std::endl;
    std::cout << "* Número lineas trazadas para el cálculo de reflejos: " 
	      << reflexe
	      << std::endl;
    std::cout << "* Número lineas trazadas para el cálculo de refracción y reflejos: " 
	      << refrac
	      << std::endl;
    std::cout << "* Número total de tests de intersección hechos: " 
	      << intersects
	      << std::endl;
    std::cout << "* Número de tests de intersección con resultado positivo y el porcentaje respecto al total: " 
	      << (double)((double)interok/(double)intersects)*100.0f << "/" << (double)(100.0f-(double)((double)interok/(double)intersects)*100.0f)
	      << std::endl;
    std::cout << "* Temps destinat: "
	      << difftime(tempsFi, tempsInici) 
	      << std::endl;
    std::cout << "* Nombre de primitives: "
	      << rt.getCountObjects()
	      << std::endl;
    std::cout << "* Nombre de llums: "
	      << rt.getCountLights()
	      << std::endl;
    std::cout << "* Resolucio de l'imatge calculada: "
	      << rt.getResolution()
	      <<  std::endl;
    
  }
};


#endif

