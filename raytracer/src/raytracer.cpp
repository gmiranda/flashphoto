#include <assert.h>
#include "raytracer.h"
#include "image.h"
#ifndef __WIN32__
	#include <unistd.h>
#endif

/*-<==>-----------------------------------------------------------------
/
/----------------------------------------------------------------------*/
CRayTracer::CRayTracer() {
  max_recursion_level = 10;
}

/*-<==>-----------------------------------------------------------------
/ Create an image, and for each pixel in the screen create a line
/ and retrieve which color is seen through this line
/ Save the image to a file
/----------------------------------------------------------------------*/
void CRayTracer::render() {
  // ...
  printf("\n                                          |\r|=00");
  CBitmap resultat(camera.getXRes(), camera.getYRes(), 24);

  CLine raig;

  for(int i=0;i<camera.getXRes();i++)
    {
      if(i%15==0)
	{
	  printf("\b\b\b=>%d",100*i/camera.getXRes());
	  fflush(stdout);
	}
      for(int j=0;j<camera.getYRes();j++)
	{
	  raig=CLine(camera.getLineAt(i,j));
	  background(raig);
	  trace(raig);

	  /*
	  // lo que MMX ho fa sol ho fem a mà :)
	  //saturació
	  raig.color.x=(raig.color.x>1)?1:raig.color.x;
	  raig.color.y=(raig.color.y>1)?1:raig.color.y;
	  raig.color.z=(raig.color.z>1)?1:raig.color.z;
	  //
	  raig.color.x=(raig.color.x<0)?0:raig.color.x;
	  raig.color.y=(raig.color.y<0)?0:raig.color.y;
	  raig.color.z=(raig.color.z<0)?0:raig.color.z;
	  */

	  resultat.setPixel(i, j,raig.color);
	}
    }

  resultat.saveTGA("res.tga");
  printf("\n\n");
}

/*-<==>-----------------------------------------------------------------
/ Find which object and at which 't' the line hits and object
/ from the scene.
/ Returns true if the object hits some object
/----------------------------------------------------------------------*/
bool CRayTracer::intersects(CLine &line)
{
  // ...
  // ^Que coño es esto?
  // Example of traversing all the objects registered in the scene
  // Same thing for lights
  LRTObjects::iterator i = objects.begin();

  //hem de buscar la mes propera
  //em sembla que aquest es el problema
  //aqui pillem el primer objecte, sigui davant o darrere,
  //tapat per un altre o no...
  SCALAR t=-1;

  while( i != objects.end() )
    {
      CRTObject *obj = *i++;
      // Si la linea intersecta al objeto
      if(obj->hits(line,t))
	{
	  if((line.t==-1)||(line.t>t)&&(t>0))
	    {
	      //std::cerr << "Inteseccion, amigo conductor" << std::endl;
	      // Guardamos el objeto con el que chocamos
	      line.obj=obj;
	      // Guardamos la distancia de intersección
	      line.t=t;
	    }
	}
    }
  if(line.t>0)
    return true;
  else
    return false;
}

/*-<==>-----------------------------------------------------------------
/ Returns in line.color the color captured by the line.
/----------------------------------------------------------------------*/
void CRayTracer::trace(CLine &line)
{
  //si no hem arribat al maxim de recursió
  if (line.getLevel()>max_recursion_level)
    return;

  //un nivell mes
  ++line;

  //fiquem com si no per inicialitzar
  line.t=-1;

  //si no intersecta no ens interesa
  if(!intersects(line))
    return;

  VECTOR pos = line.getIntersection();

  //Ambiental suposem que hi es encara que no llum
  line.addColor(line.obj->getMaterial()->getDiffuseColor(pos)
		*0.5*
		(1-line.obj->getMaterial()->getReflectance(pos)));

  //llums
  std::list<CLight *>::iterator llum;
  for(llum = lights.begin();llum!=lights.end();++llum)
    {
      // Vector L
      VECTOR L = (*llum)->getLocation()-pos;
      L.normalize();
      CLine llumLinea((*llum)->getLocation(),L, 0);

      VECTOR N = line.obj->getNormal(pos);
      SCALAR NL=N.dot(L);

      if(NL<0) NL=0;

      //llum difosa

      line.addColor(line.obj->getMaterial()->getDiffuseColor(pos)
		    *
		    (NL)
		    *
		    (1-line.obj->getMaterial()->getReflectance(pos)));


      //llum especular
      VECTOR E;
      E=-line.dir;
      //E.normalize();
      // La L paralela
      VECTOR Lpar =N*(N.dot(L));
      // La L perpendicular
      VECTOR Lper = (L-Lpar);
      VECTOR R = Lpar-Lper;
      // En principio no hace falta
	  //R.normalize();
      // cos Beta = RE, se calcula asi :)
      SCALAR RE=R.dot(E);
      // Si es negativo, no hay componente especular
      if(RE>=0){
          // Especular = Is*(cos Beta)^n por Ks
          // Dice que podemos sudar de Is y Ks xD
          COLOR especular=VECTOR(1.0,1.0,1.0)
            *pow(RE,21)/**0.8f*/;
            //std::cerr << "Especular="<<especular << std::endl;
          line.addColor(especular);
          /*line.addColor(line.obj->getMaterial()->getDiffuseColor(pos)
                *-pow(RE,20)*
                (1-line.obj->getMaterial()->getReflectance(pos)));*/
      }


      // Luz reflejada
      /*if(line.obj->getMaterial()->getReflectance(pos)>0)
	{
	  CLine reflexe;

	  line.t=-1;

	  reflexe= line.getReflected(pos,line.obj->getNormal(pos) );
	  reflexe.color.x=0;
	  reflexe.color.y=0;
	  reflexe.color.z=0;
	  trace(reflexe);

	  //com l'afegeixo?
	  line.addColor(reflexe.color);
	}*/
    }
}

/*-<==>-----------------------------------------------------------------
/ Default background
/----------------------------------------------------------------------*/
void CRayTracer::background(CLine &line)
{
  line.color = COLOR (0,0,0);
}

