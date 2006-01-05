#ifndef _Shadow_h_
#define _Shadow_h_

#include "CImg.h"
using namespace cimg_library;

// Metodos de ayuda
#include "Helper.h"

// Clase Pixel para el filtro de correcion de color.
#include "Pixel.h"

// Biblioteca de calculos matematicos
#include <cmath>

// Biblioteca estandar
#include <iostream>
using std::cerr;
using std::endl;

/**
 * Clase para la detección de sombras.
 * 
 */
class Shadow{

public:
	// Deteccion de umbra
	static float umbraCandidate(const CImg<float>& histo);
	static CImg<bool>& dischardPixels(const CImg<>& image, const float threshold);
	static CImg<bool>& umbra(const CImg<>& intenF, const CImg<>& intenNF);
	
	// Deteccion de penumbra
	static CImg<bool> penumbra(const CImg<>& intenF, const CImg<>& intenNF, const CImg<bool>& umbra); 
	
	// Procesa la sombra
	static CImg<bool>& shadow(const CImg<>& intenF, const CImg<>& intenNF);
	
	// Correcion de color y sombra
	static CImg<>& colorCorrection(const CImg<>& noFlash,const CImg<>& flash,const CImg<bool>& shadow);
	static CImg<>& detailsCorrection(const CImg<>& detailsCorrected, const CImg<>& detailsNoFlash, const CImg<bool>& shadow);

	
};


#endif
