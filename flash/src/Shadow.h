#ifndef _Shadow_h_
#define _Shadow_h_

// Copyright (C) 2005  Guillermo Miranda Alamo
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

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
using std::cout;
using std::cerr;
using std::endl;

/**
 * Clase para la detección de sombras.
 * ToDo: create a constructor that stores a reference to the
 * intensities (inten and IntenNF).
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

/**
 * Experimental Shadow class,
 * uses faster algorithms, but may produce worse results.
 * @see Shadow
 */
class ShadowExperimental : public Shadow{
public:
	static CImg<>& colorCorrection(const CImg<>& noFlash,const CImg<>& flash,const CImg<bool>& shadow);
};


#endif
