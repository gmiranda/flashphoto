#ifndef INC_OBJECTS_H_
#define INC_OBJECTS_H_

#include "line.h"

/*-<==>-----------------------------------------------------------------
/ Generic Object to ray trace
/---------------------------------------------------------------------*/
class CRTObject {

protected:
  VECTOR 		loc;						// Position of the object
  CMaterial	*material;      // Current material of the object. The materil
                            // object pointed is not owned by the this CRTObject

public:
	// Constructor & Destructor
  CRTObject();
  virtual ~CRTObject();

	// Get/Set members common to all objects
	// TODO (Guille#1#): Esto peta, hay que arreglarlo.\
	He investigado y tiene una direccion de memoria.\
	He mirado si es cosas de casting, pero tampoco.\
	A lo mejor tu tienes mas suerte
  CMaterial *getMaterial() const { return material; }
  void    setMaterial (CMaterial *new_material);
  void    setLocation (const VECTOR &loc);

	// Derived clases must define these pure virtual methods:

	// Returns true if the given line hits the object, setting t_hit
	// to the value of t where the nearest intersection is found
	// Returns false if no intersection is found between the object
	// and the line
  virtual bool hits(const CLine &line, SCALAR &t_hit) = 0;

	// Return the normal on the surface specified by position loc
  virtual VECTOR getNormal(const VECTOR &loc) = 0;
};

// Define a list of pointers to objects
#include <list>
typedef std::list<CRTObject *> LRTObjects;

#endif
