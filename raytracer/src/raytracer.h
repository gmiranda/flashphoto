#ifndef INC_RAY_TRACER_H_
#define INC_RAY_TRACER_H_

#include <iostream>
#include "geometry.h"
#include "material.h"
#include "rtobject.h"
#include "camera.h"
#include "light.h"

#define SMALL_AMOUNT      1e-5

/*-<==>-----------------------------------------------------------------
/ Raytracer
/---------------------------------------------------------------------*/
class CRayTracer {
	// Maximum recursion level allowed while computing 
	// lines 
  int         max_recursion_level;

  CCamera     camera; 			// The camera to render the scene from
  LRTObjects  objects; 			// List of objects defined in the scene
  MMaterials  materials; 		// Map of materials defined in the scene
  LLights     lights; 			// List of lights defined in the world

  void trace (CLine &line);
  bool intersects (CLine &line);
  void background (CLine &line);
public:
  CRayTracer();
  void render();
  void load();              // defined in main.cpp
  bool loadSnowflake (const char *filename);
};

// Send a vector to a output stream
std::ostream &operator << (std::ostream &os, const VECTOR &v);

#endif
