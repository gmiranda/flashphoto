#ifndef _CImg_h_
#define _CImg_h_

/**
 * This file tells CImg where it should look
 * for plugins, so there's no need to keep
 * adding this code on each CImg release.
 *
 * Of couse, it also includes the real CImg header file.
 */

/**
 * Note that ../ is mandatory, since CImg.h (the big one) is
 * inside the CImg folder
 */
#define cimg_plugin "../CImg_plugins.cpp"
#define cimg_use_png
#define cimg_use_jpeg

#include "CImg/CImg.h"

#endif
