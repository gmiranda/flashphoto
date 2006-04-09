#include <cassert>
#include "image.h"

/*-<==>-----------------------------------------------------------------
/  
/----------------------------------------------------------------------*/
int  CBitmap:: saveTGA (const char *filename, bool compress) {
unsigned char tgah[18];
FILE *f;
int i;

  f = fopen (filename, "wb");
  if (!f) return -1;
  memset (tgah, 0x00, 18);
  tgah[2]  = (compress) ? 0x0a : 0x02;
  tgah[12] = (char) (getXRes() & 0xFF);          // Resolution
  tgah[13] = (char) (getXRes() >> 8);
  tgah[14] = (char) (getYRes() & 0xFF);          
  tgah[15] = (char) (getYRes() >> 8);
  tgah[16] = getBPP();                            // Color depth
  if (tgah[16] == 15) tgah[16] = 16;              // Format conventions
  tgah[17] = 0x00;                                // up..dw & lf..rt
  fwrite (tgah, 18, 1, f);                        // Save Header

  if (tgah[2] == 0x02) {                          // Uncompressed
    for (i=0; i<getYRes(); i++)
      fwrite ((BYTE *) getBuffer() + (getYRes()-1-i)*getPitch(), getXRes(), getBytesPerPixel(), f);
    }
  else {
    const int n = getBytesPerPixel();
    for (i=0; i<getYRes(); i++) {
      BYTE *c = (BYTE *) getBuffer() + (getYRes()-1-i)*getPitch();
      int count = getXRes();
      while (count) {
        BYTE *db = c;
        int ndistintos = 0;
        while (count>1) {
          if (memcmp (c, c+n, n) == 0) break;
          c += n;
          count--;
          ndistintos++;
          }
        BYTE *ib = c;
        int niguales = 0;
        while (count) {
          if (count > 1 && memcmp (c, c+n, n) != 0) {
            c += n;
            count--;
            niguales++;
            break;
            }
          c += n;
          count--;
          niguales++;
          }
        while (ndistintos) {
          BYTE kp,np;
          if (ndistintos > 0x80) np = 0x80;
          else                   np = ndistintos;
          kp = np - 1;
          fwrite (&kp,  1, 1, f);
          fwrite ( db, np, n, f);
          db += np*n; 
          ndistintos -= np;
          }
        while (niguales) {
          BYTE np,kp;
          if (niguales > 0x80) np = 0x80;
          else                 np = niguales;
          kp = (np - 1) + 0x80;
          fwrite (&kp, 1, 1, f);
          fwrite ( ib, 1, n, f);
          niguales -= np;
          }
        }
      }
    }
  
  fclose (f);
  return 0;
}


/*-<==>-----------------------------------------------------------------
/  
/----------------------------------------------------------------------*/
void CBitmap:: setPixel(int x, int y, const COLOR &color) {
	// Check limits of the coordinates
	assert ( x >= 0 && x < xres );
	assert ( y >= 0 && y < yres );
	// Only suppoting 8 bits / component
	assert ( bpp == 24 );

	// Address of the pixel
	COLOR3 *img_buffer = (COLOR3 *) getBuffer();
	img_buffer += y * getDeltaScan() + x;

	// Update the frame buffer
	img_buffer->r = (int) (color[0] * 255);
  img_buffer->g = (int) (color[1] * 255);
  img_buffer->b = (int) (color[2] * 255);
}
