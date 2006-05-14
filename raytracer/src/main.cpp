#include "raytracer.h"
#include "sphere.h"
#include "plane.h"
#include "cylinder.h"
#include <iostream>
#include "estats.hxx"
#include "cmaterial.hxx"

/*-<==>-----------------------------------------------------------------
  / Defines the scene
  /----------------------------------------------------------------------*/
void CRayTracer::load () {

  // Add the camera looking at the origin
  camera.setView (VECTOR(0, 50, 400), VECTOR (0,0,0));
  camera.setRenderParameters (640,480,60);

  // Define some materials
  materials["orange"]  = new CSolidMaterial (COLOR (1, 0.301, 0.074), 0.1);
  materials["blue"]    = new CSolidMaterial (COLOR (0.0, 0.2, 0.8), 0.0);
  //
  materials["red"]    = new CSolidMaterial (COLOR (0.8, 0.2, 0.0), 0.5);
  materials["green"]    = new CSolidMaterial (COLOR (0.1, 0.8, 0.0), 0.7);

  materials["grey"]    = new CSolidMaterial (COLOR (0.5, 0.5, 0.5), 0.5);

  materials["escacs"]  = new CCheckerMaterial(materials["red"],
					     materials["green"],100);

  materials["glass"] = new CTransparentMaterial(COLOR(0.1,0.1,0.2),1);

  // Add a sphere
  CSphere *sph = new CSphere(50);
  sph->setLocation (VECTOR(0,-100,0));
  sph->setMaterial (materials["grey"]);
  objects.push_back (sph);

  // Add a sphere
  sph = new CSphere(50);
  sph->setLocation (VECTOR(0,100,0));
  sph->setMaterial (materials["blue"]);
  objects.push_back (sph);

  // Add a sphere
  sph = new CSphere(50);
  sph->setLocation (VECTOR(0,200,0));
  sph->setMaterial (materials["red"]);
  objects.push_back (sph);

    // Add a sphere
  sph = new CSphere(50);
  sph->setLocation (VECTOR(350,50,0));
  //sph->setMaterial (materials["glass"]);
  sph->setMaterial (materials["green"]);
  objects.push_back (sph);

  // Add a sphere
  sph = new CSphere(50);
  sph->setLocation (VECTOR(200,50,0));
  sph->setMaterial (materials["orange"]);
  objects.push_back (sph);

  // And now for something completely different
//   CCylinder* cyl = new CCylinder(100,50);
//   cyl->setLocation (VECTOR(85,0,120));
//   cyl->setMaterial (materials["grey"]);
//   objects.push_back(cyl);
  /*CCylinder* cyl = new CCylinder(100,50);
  cyl->setLocation (VECTOR(150,0,100));
  cyl->setMaterial (materials["green"]);
  objects.push_back(cyl);*/


  // Add the ground
  CPlane *plane = new CPlane (VECTOR(0,1,0), 0);
  plane->setMaterial (materials["escacs"]);
  objects.push_back (plane);

  // Add a single white light
  CLight *light = new CLight(VECTOR (400,200,50), COLOR (1,1,1));
  lights.push_back (light);
}

bool CRayTracer::loadSnowflake (const char *filename)
{
  FILE *f = fopen (filename, "r");
  if (!f)
    return false;

  // Add the camera looking at the origin
  camera.setView (VECTOR(1.05, 0.85, 0.65), VECTOR (0,0,0));
  camera.setRenderParameters (512,512,45);

  // Add a two material
  materials["txt001"]    = new CSolidMaterial (COLOR (0.8, 0.6, 0.264), 0);
  materials["txt002"]    = new CSolidMaterial (COLOR (0.5, 0.45, 0.35), 0.5);

  // Add the ground
  CPlane *plane = new CPlane (VECTOR(0,1,0), -0.5);
  plane->setMaterial (materials["txt001"]);
  objects.push_back (plane);

  // This is a very simply parser!!
  while (!feof(f)) {
    char buf[512];
    fgets (buf, 511, f);
    if (strncmp (buf, "sphere", 6) == 0) {
      char material[64];
      double x,y,z, rad;
      sscanf (buf, "sphere %s %lf %lf %lf %lf\n", material, &rad, &x,&y,&z);
      CSphere *sph = new CSphere(rad);
      sph->setLocation (VECTOR(x,z,y));
      sph->setMaterial (materials["txt002"]);
      objects.push_back (sph);
    }
  }

  // Add 3 white lights
  lights.push_back (new CLight(VECTOR ( 4, 3, 2), COLOR (1,1,1)));
  lights.push_back (new CLight(VECTOR ( 1,-4, 4), COLOR (1,1,1)));
  lights.push_back (new CLight(VECTOR (-3, 1, 5), COLOR (1,1,1)));

  fclose (f);
  return true;
}


Estats Estats::inst;

/*-<==>-----------------------------------------------------------------
/ MAIN
/----------------------------------------------------------------------*/
int main(int argc, char **argv)
{
  std::cout << "Tracing..." << std::endl;
  CRayTracer rt;

  //despres ja ho canviarem a la seva manera
  // Use filename given as runtime argument
  if (argc>1)
    rt.loadSnowflake(argv[1]);
  else
    rt.load();

  Estats::getInstance().start();
  rt.render();
  Estats::getInstance().stop();

  Estats::getInstance().printStats(rt);

  return 0;
}

