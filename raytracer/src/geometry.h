#ifndef INC_GEOMETRY_H_
#define INC_GEOMETRY_H

// Disable warning from VC++ when compiling some templates
#pragma warning (disable : 4786 )

// Read 3D vector template definition
#include "vector_template.h"

typedef double SCALAR;
typedef T_VECTOR3<SCALAR> VECTOR;
typedef VECTOR COLOR;

#ifndef M_PI
#define M_PI 3.14159265358979323846f 
#endif

#endif
