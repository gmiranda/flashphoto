#ifndef INC_BITMAP_H_
#define INC_BITMAP_H_

#include <string.h>
#include <cstdio>
#include "geometry.h"

/*-<==>-----------------------------------------------------------------
/ Generic Bitmap
/----------------------------------------------------------------------*/
class CBitmap {
protected:
  int   xres, yres;         // resolution
  int   bpp;                // bits per pixel, 15,16,24,32
  int   bytesperpixel;      // bytes per pixel
  int   imgsize;            // in bytes
  unsigned char *bufRoot;   // buffer;
  int   deltascan;          // in pixel units, not byte units!
  bool  owner;							// If the instance is the owner if the buffer
public:
	// Copy constructor. The memory buffer is not duplicated!!
  CBitmap(const CBitmap &w) {
    xres = w.xres;
    yres = w.yres;
    bpp  = w.bpp;
    bytesperpixel = w.bytesperpixel;
    imgsize = w.bytesperpixel;
    bufRoot = w.bufRoot;
    owner = false;
  }
	// Copy constructor
  CBitmap(int nxres, int nyres, int nbpp) {
    setDimensions (nxres, nyres, nbpp);
    bufRoot = new unsigned char [imgsize];
    owner = true;
  }
	// Create a dummy empty image
  CBitmap() {
    setDimensions (0,0,0);
    owner = false;
    bufRoot = 0L;
  }
	// Forces the release of the image buffer
  virtual ~CBitmap() {
    setBuffer (0L);
  }
	// access methods
  void *getBuffer ()   const { return bufRoot; }      // Returns base buffer pointer
  int   getXRes()      const { return xres; }         // Width in pixels. 
  int   getYRes()      const { return yres; }         // Height in pixels
  int   getBPP()       const { return bpp; }          // bits per pixel 8,16,24,32
  int   getPitch()     const { return deltascan*bytesperpixel; }  // Just in case. Pitch means in bytes
  int   getDeltaScan() const { return deltascan; }    // In pixel units
  int   getImgSize()   const { return imgsize; }      // xres*yres*bytesperpixel
  int   getBytesPerPixel() const { return bytesperpixel; } // bits per pixel 8,16,24,32
  void  setDeltaScan(int ns) { deltascan = ns; }      // In pixel units
	void  setPixel(int x, int y, const COLOR &color);

	// Sets the new buffer to be used by the instance.
  void  setBuffer (void *new_buffer) {			
		// Release prev buffer if set
    if (owner)
      delete [] bufRoot;
    bufRoot = (unsigned char *) new_buffer; 
    owner = false;
    }
  void  setOwner (bool how) { owner = how; }          // 
  void  setDimensions (int nxres, int nyres, int nbpp) {
    xres = nxres;
    yres = nyres;
    bpp  = nbpp;
    bytesperpixel = (nbpp+1) >> 3; 
    imgsize = xres*yres*bytesperpixel;
    setDeltaScan (xres);
    }
  int   saveTGA (const char *filename, bool compress = true);  
  };

/*-<==>-----------------------------------------------------------------
/ Some definitions 
/----------------------------------------------------------------------*/
typedef unsigned char BYTE;
typedef struct {BYTE b,g,r;} COLOR3;
typedef unsigned int COLOR4;
typedef struct {BYTE b,g,r,alfa;} COLOR4_RGBA;

#endif
