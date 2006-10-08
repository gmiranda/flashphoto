#ifndef _FastDecoupling_h_
#define _FastDecoupling_h_

/**
 * @file FastDecoupling header file.
 */

// Include Decoupling base class
#include "Decoupling.h"

/**
 * @class Decoupling subclass that uses a Signal Processing Approach
 * bilateral filter, described by Sylvain Paris and Frédo Durand.
 * @see http://people.csail.mit.edu/sparis/
 * @ToDo: Use log10 inside the bilateral filter, as the original Flash paper says.
 */
class FastDecoupling : public Decoupling{
	//! @brief Clamp
	template<typename Real,typename RealDummy1,typename RealDummy2>
  	inline Real clamp(const RealDummy1 min_value, const RealDummy2 max_value,
		    const Real x)const;

	//! @brief Trilinear interpolation.
	float trilinearInterpolation(const CImg<>& array, const float x,
		const float y, const float z)const;


public:
	/**
	 * \section Image decoupling methods
	 */
	CImg<>& bilateralFilter(const CImg<>& I)const;
	CImg<>& bilateralFilter(const CImg<>& I, const CImg<bool>& shadow)const;
	CImg<>& crossBilateralFilter(const CImg<>& iNF,const CImg<>& iF)const;

	// Metodos para la reconstruccion
	//CImg<float>& reconstruct(const CImg<>& color, const CImg<>& details, const CImg<> largeScaleNF)const;

	/**
	 * \section Constructors & destructors
	 */
	FastDecoupling(const CImg<>& imageFlash, const CImg<>& imageNoFlash);
	virtual ~FastDecoupling();
};

#endif // _FastDecoupling_h_
