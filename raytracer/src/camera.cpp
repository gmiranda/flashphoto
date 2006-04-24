#include "raytracer.h"
#include <math.h>

/*-<==>-----------------------------------------------------------------
/
/---------------------------------------------------------------------*/
CCamera::CCamera() {
	// Initialize with some default parameters
  setRenderParameters(320, 240, 60.0);
	setView(VECTOR(0,0,0), VECTOR(0,0,1));
}

CCamera::~CCamera() {
}

/*-<==>-----------------------------------------------------------------
/ Define render parameters
/ fov is in degrees
/---------------------------------------------------------------------*/
void CCamera::setRenderParameters (int axres, int ayres, SCALAR afov)
{
  //:)
  if(afov<0)
    {
      while(afov<0)
	afov+=360;
    }
  else
    {
      while(afov>360)
	afov-=360;
    }

  // happy parameters
  fov=(afov*M_PI)/180; //rad
  xres=axres;
  yres=ayres;
  // ...
}

/*-<==>-----------------------------------------------------------------
/ Define the axis of the camera (front, up, left) in world coordinates
/ based on the current values of the vectors target & loc
/---------------------------------------------------------------------*/
void CCamera::initAxis()
{
  // ...
  viewd=(xres/2)/(tan(fov/2));
  //
  front=target-loc;
  front.normalize();
  //
  VECTOR vertical=VECTOR(0,1,0);
  left=vertical.cross(front);
  left.normalize();
  //
  up=front.cross(left);
  up.normalize();
  //
}

/*-<==>-----------------------------------------------------------------
/ Save the new camera position and target point
/---------------------------------------------------------------------*/
void CCamera::setView(const VECTOR &src_point, const VECTOR &dst_point)
{
  loc = src_point;
  target = dst_point;
  initAxis();
}

/*-<==>-----------------------------------------------------------------
/ return a line which starts on camera position and goes through the pixel
/ (x,y) from the screen
/---------------------------------------------------------------------*/
CLine CCamera::getLineAt (SCALAR x, SCALAR y)
{
  // ...
  SCALAR MAX_X=xres/2;
  SCALAR MAX_Y=yres/2;
  //
  SCALAR MIN_X=-xres/2;
  SCALAR MIN_Y=-yres/2;

  x=((xres/2)-x);
  y=((yres/2)-y);

  assert((x>=MIN_X)&&(x<=MAX_X));
  assert((y>=MIN_Y)&&(y<=MAX_Y));

  VECTOR tmp=viewd*front;

  tmp+=x*left;
  tmp+=y*up;

  tmp.normalize();

  return CLine(loc,tmp);
}

