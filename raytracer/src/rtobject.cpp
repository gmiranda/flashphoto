#include "raytracer.h"

CRTObject::CRTObject() : loc(0,0,0), material(NULL) {
	// Init members
}

CRTObject::~CRTObject() {
	// Nothing special to be done
}

/*-<==>-----------------------------------------------------------------
/ Save new position of the object
/----------------------------------------------------------------------*/
void CRTObject::setLocation(const VECTOR &new_loc) {
  loc = new_loc;
}

/*-<==>-----------------------------------------------------------------
/ Save the pointer to the current material of this object
/----------------------------------------------------------------------*/
void CRTObject::setMaterial (CMaterial *new_material) {
	material = new_material;
}
