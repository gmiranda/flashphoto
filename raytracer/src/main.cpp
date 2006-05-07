#include "raytracer.h"
#include "sphere.h"
#include "plane.h"
#include "cylinder.h"
#include <iostream>

/*-<==>-----------------------------------------------------------------
/ Defines the scene
/----------------------------------------------------------------------*/
void CRayTracer::load () {

  // Add the camera looking at the origin
  camera.setView (VECTOR(300, 120, 120), VECTOR (0,0,0));
  camera.setRenderParameters (640,480,60);

  // Define some materials
  materials["orange"]  = new CSolidMaterial (COLOR (1, 0.301, 0.074), 0);
  materials["blue"]    = new CSolidMaterial (COLOR (0.0, 0.2, 0.8), 0);
  //
  materials["red"]    = new CSolidMaterial (COLOR (0.8, 0.2, 0.0), 0);
  materials["green"]    = new CSolidMaterial (COLOR (0.1, 0.8, 0.0), 0);

  // Add a sphere
  CSphere *sph = new CSphere(50);
  sph->setLocation (VECTOR(0,50,0));
  sph->setMaterial (materials["red"]);
  objects.push_back (sph);

  // Add a sphere
  sph = new CSphere(50);
  sph->setLocation (VECTOR(0,150,0));
  sph->setMaterial (materials["green"]);
  objects.push_back (sph);

  // Add a sphere

  sph = new CSphere(50);
  sph->setLocation (VECTOR(0,250,0));
  sph->setMaterial (materials["blue"]);
  objects.push_back (sph);

  // And now for something completely different
  CCylinder* cyl = new CCylinder(100,50);
  //cyl->setLocation(VECTOR(50,25,10));
  cyl->setLocation (VECTOR(0,0,0));
  cyl->setMaterial (materials["blue"]);
  objects.push_back(cyl);

  // Add the ground
  CPlane *plane = new CPlane (VECTOR(0,1,0), 0);
  plane->setMaterial (materials["orange"]);
  objects.push_back (plane);

  // Add a single white light
  CLight *light = new CLight(VECTOR (400,400,400), COLOR (1,1,1));
  lights.push_back (light);

}

/*-<==>-----------------------------------------------------------------
/ MAIN
/----------------------------------------------------------------------*/
int main(int argc, char **argv)
{
  std::cout << "GayTracer is tracing..." << std::endl;
  CRayTracer rt;
  rt.load();
  rt.render();
  std::cout << "is behind you!" << std::endl;
  return 0;
}

