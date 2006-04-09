#ifndef INC_CAMERA_H_
#define INC_CAMERA_H_

/*-<==>-----------------------------------------------------------------
/ General Camera definition
/---------------------------------------------------------------------*/
class CCamera {
  VECTOR loc, target;	   // Where is the camera, and where is looking at
  VECTOR front, left, up;  // Camera axis in world coordinates
  SCALAR fov;//		   // Visual angle
  SCALAR viewd;//	   // Distance to the screen
  int    xres;//	   // Screen resolution in pixels
  int    yres;//
  void initAxis();
public:
	// Constructor/Destructor
  CCamera();
  virtual ~CCamera();

	// Set the render parameteres
  void setRenderParameters (int axres, int ayres, SCALAR afov);
	// Define the camera orientation
  void setView (const VECTOR &src_point, const VECTOR &dst_point);
	// read parameters
  int  getXRes() const { return xres; }
  int  getYRes() const { return yres; }
	// Returns a line passing throught the camera loc and the
	// screen coordinates (x,y)
  CLine getLineAt (SCALAR x, SCALAR y);
};

#endif
