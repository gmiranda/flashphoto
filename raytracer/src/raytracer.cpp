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
	  sleep(1); //pq es vegi
	}
      for(int j=0;j<camera.getYRes();j++)
	{
	  raig=new CLine(camera.getLineAt(i,j));

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
/----------------------------------------------------------------------*/
bool CRayTracer::intersects(CLine &line) {
	// ...

	// Example of traversing all the objects registered in the scene
	// Same thing for lights
	LRTObjects::iterator i = objects.begin();
	while( i != objects.end() ) {
		CRTObject *obj = *i++;
		// At this point we can use obj->method...

	}
}

/*-<==>-----------------------------------------------------------------
/ Returns in line.color the color captured by the line.
/----------------------------------------------------------------------*/
void CRayTracer::trace(CLine &line) {
	// ...
}

/*-<==>-----------------------------------------------------------------
/ Default background
/----------------------------------------------------------------------*/
void CRayTracer::background(CLine &line) {
  line.color = COLOR (0,0,0);
}

