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
#include "CImg.h"
// Usamos su namespace
using namespace cimg_library;

// Para trabajar con pixeles incluimos:
#include "Pixel.h"

// Y los metodos auxiliares
#include "Helper.h"



/**
 * Class that handles decoupling & reconstruction of the images
 * @author Guillermo Miranda Alamo <gmiranda@teleline.es>
 * ToDo: comprobar si tengo que trabajar con la imagen en YUV
 * y no en RGB.
 */
class Decoupling{
	/// Imagen capturada con flash, de la cual obtendremos color y detalles
	CImg<float> _imageFlash;

	/// Imagen capturada sin flash, la usaremos para sacar la imagen Large Scale
	CImg<float> _imageNoFlash;
protected:
	/// Varianza espacial
	const float _sigmaF;

	/// Varianza para la influencia de la intensidad
	const float _sigmaG;

public:
	// Bilateral filters
	virtual CImg<float>& bilateralFilter(const CImg<float>& I)const=0;
	virtual CImg<float>& bilateralFilter(const CImg<float>& I, const CImg<bool>& shadow)const=0;
	virtual CImg<float>& crossBilateralFilter(const CImg<float>& iNF,const CImg<float>& iF)const=0;
	
	// Layer methods
	CImg<float>& getIntensity(const CImg<float>& image)const;
	CImg<float>& getColor(const CImg<float>& intensity)const;
	CImg<float>& getColor(const CImg<>& image, const CImg<float>& intensity)const;

	//! Reconstructs the image
	CImg<float>& reconstruct(const CImg<>& color, const CImg<>& details, const CImg<> largeScaleNF)const;
	
	// Constructors & destructors
	Decoupling(const CImg<>& imageFlash, const CImg<>& imageNoFlash);
	virtual ~Decoupling();
};
/**
 * Stable, initial and SLOW algorithms.
 * However, they produce the best results, really good ones.
 * If you have a lot of CPU power and time, use this,
 * otherwise you'd better off with the experimental namespace.
 */
class DecouplingStable : public Decoupling{

public:
	CImg<float>& bilateralFilter(const CImg<float>& I)const;
	CImg<float>& bilateralFilter(const CImg<float>& I, const CImg<bool>& shadow)const;
	CImg<float>& crossBilateralFilter(const CImg<float>& iNF,const CImg<float>& iF)const;
	/*static CImg<>& gaussianKernel(const int size=5, const float sigma=1.0f);
	CImg<>& interpolationWeight(const CImg<>& I, const float ij)const;
	CImg<>& piecewiseBilateralFilter(const CImg<float>& I)const;
	CImg<float>& getIntensity(const CImg<float>& image)const;
	CImg<float>& getColor(const CImg<float>& intensity)const;
	CImg<float>& getColor(const CImg<>& image, const CImg<float>& intensity)const;*/

	// Metodos para la reconstruccion
	//CImg<float>& reconstruct(const CImg<>& color, const CImg<>& details, const CImg<> largeScaleNF)const;
	
	// Constructors & destructors
	DecouplingStable(const CImg<>& imageFlash, const CImg<>& imageNoFlash);
	virtual ~DecouplingStable();
};


/**
 * Subclass that uses faster, experimental algorithms.
 * @ToDo: Use log10 inside the bilateral filter, as the paper says.
 */
class DecouplingExperimental : public Decoupling{
public:
	// Metodos para obtener las imagenes
	CImg<float>& bilateralFilter(const CImg<float>& I)const;
	CImg<float>& bilateralFilter(const CImg<float>& I, const CImg<bool>& shadow)const;
	CImg<float>& crossBilateralFilter(const CImg<float>& iNF,const CImg<float>& iF)const;
	static CImg<>& gaussianKernel(const int size=5, const float sigma=1.0f);
	CImg<>& interpolationWeight(const CImg<>& I, const float ij)const;
	CImg<>& piecewiseBilateralFilter(const CImg<float>& I)const;
	/*CImg<float>& getIntensity(const CImg<float>& image)const;
	CImg<float>& getColor(const CImg<float>& intensity)const;
	CImg<float>& getColor(const CImg<>& image, const CImg<float>& intensity)const;*/
	
	// Metodos para la reconstruccion
	//CImg<float>& reconstruct(const CImg<>& color, const CImg<>& details, const CImg<> largeScaleNF)const;

	// Constructors & destructors
	DecouplingExperimental(const CImg<>& imageFlash, const CImg<>& imageNoFlash);
	virtual ~DecouplingExperimental();
};

#endif
