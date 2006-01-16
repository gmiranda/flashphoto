#ifndef _Decoupling_h_
#define _Decoupling_h_

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

#include <iostream>
using std::cerr;
using std::endl;

#include <cmath>

// Biblioteca de CImg
#include "CImg/CImg.h"
// Usamos su namespace
using namespace cimg_library;

// Para trabajar con pixeles incluimos:
#include "Pixel.h"

// Y los metodos auxiliares
#include "Helper.h"

/**
 * Clase que se encarga del decoupling y de la reconstruccion
 * de las imagenes.
 * @author Guillermo Miranda Alamo <guillermo.miranda01@campus.upf.es>
 * @version 1.0
 * ToDo: comprobar si tengo que trabajar con la imagen en YUV
 * y no en RGB.
 */
class Decoupling{
	/// Imagen capturada con flash, de la cual obtendremos color y detalles
	CImg<float> _imageFlash;

	/// Imagen capturada sin flash, la usaremos para sacar la imagen Large Scale
	CImg<float> _imageNoFlash;

	/// Varianza espacial
	const float _sigmaF;

	/// Varianza para la influencia de la intensidad
	const float _sigmaG;


public:
	// Metodos para obtener las imagenes
	CImg<float>& bilateralFilter(const CImg<float>& I)const;
	CImg<float>& bilateralFilterAlt(const CImg<float>& I)const;
	CImg<float>& bilateralFilter(const CImg<float>& I, const CImg<bool>& shadow)const;
	CImg<float>& crossBilateralFilter(const CImg<float>& iNF,const CImg<float>& iF)const;
	CImg<float>& crossBilateralFilterAlt(const CImg<float>& iNF,const CImg<float>& iF)const;
	static CImg<>& gaussianKernel(const int size=5, const float sigma=1.0f);
	CImg<>& interpolationWeight(const CImg<>& I, const float ij)const;
	CImg<>& piecewiseBilateralFilter(const CImg<float>& I)const;
	CImg<float>& getIntensity(const CImg<float>& image)const;
	CImg<float>& getColor(const CImg<float>& intensity)const;
	CImg<float>& getColor(const CImg<>& image, const CImg<float>& intensity)const;

	// Metodos para la reconstruccion
	CImg<float>& reconstruct(const CImg<>& color, const CImg<>& details, const CImg<> largeScaleNF)const;


	// Constructores y destructores
	Decoupling(const CImg<>& imageFlash, const CImg<>& imageNoFlash);
	virtual ~Decoupling();
};

#endif
