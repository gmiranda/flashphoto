#include <assert.h>
#include "raytracer.h"
#include "image.h"

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
  printf("\n                                             |\r|=");
  CBitmap resultat(camera.getXRes(), camera.getYRes(), 24);

  CLine *raig;

  for(int i=0;i<camera.getXRes();i++)
    {
      if(i%15==0)
	{
	  printf("\b=>");
	  fflush(stdout);
	  // TODO (Guille#9#): para que compile en win32 :P
	  //sleep(1); //pq es vegi
	}
      for(int j=0;j<camera.getYRes();j++)
	{
	  raig=new CLine(camera.getLineAt(i,j));
	  background(*raig);
	  trace(*raig);

	  // lo que MMX ho fa sol ho fem a mà :)
	  //saturació
	  raig->color.x=(raig->color.x>1)?1:raig->color.x;
	  raig->color.y=(raig->color.y>1)?1:raig->color.y;
	  raig->color.z=(raig->color.z>1)?1:raig->color.z;
	  //
	  raig->color.x=(raig->color.x<0)?0:raig->color.x;
	  raig->color.y=(raig->color.y<0)?0:raig->color.y;
	  raig->color.z=(raig->color.z<0)?0:raig->color.z;

	  resultat.setPixel(i, j,raig->color);
	  delete raig;
	}
    }

  resultat.saveTGA("res.tga");
  printf("\n\n");
}

/*-<==>-----------------------------------------------------------------
/ Find which object and at which 't' the line hits and object
/ from the scene.
/ Returns true if the object hits some object
\ Nota: sera 'if the line hits some object' o si acaso any
http://www.amazon.com/gp/product/B00004R8L6/qid=1145724283/sr=8-1/ref=pd_bbs_1/002-8316345-3944001?%5Fencoding=UTF8&v=glance&n=229534
/----------------------------------------------------------------------*/
bool CRayTracer::intersects(CLine &line)
{
  // ...
  // ^Que coño es esto?
  // Example of traversing all the objects registered in the scene
  // Same thing for lights
  LRTObjects::iterator i = objects.begin();
  while( i != objects.end() )
    {
      CRTObject *obj = *i++;
      // At this point we can use obj->method...
      // TODO (Guille#1#): No tengo ni idea de que pasarle como segundo param
      // (Guille): Creo que nos devuelve donde colisiona
      SCALAR t;
      // Si la linea intersecta al objeto
      if(obj->hits(line,t)){
	//std::cerr << "Inteseccion, amigo conductor" << std::endl;
	// Guardamos el objeto con el que chocamos
	line.obj=obj;
	// Guardamos la distancia de intersección
	line.t=t;
	// Guay
	return true;
      }
    }

  // Mala suerte...
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

  //si no intersecta no ens interesa
  if(!intersects(line))
    return;

  VECTOR pos = line.getIntersection();

  //Ambiental
  line.addColor(COLOR(0.2,0.2,0.2));

  // Obtenemos el material del objeto
  CMaterial* mat = line.obj->getMaterial();
  assert(mat!=0);

  // Cogemos el color difuso que da esta linea per la pos
  line.addColor(mat->getDiffuseColor(pos));

  //Difusa
/*SCALAR NL=line.obj.dot(line.dir);
if(NL>0)
{
line.addColor()
}*/
}

/*-<==>-----------------------------------------------------------------
/ Default background
/----------------------------------------------------------------------*/
void CRayTracer::background(CLine &line)
{
  line.color = COLOR (0,0,0);
}

