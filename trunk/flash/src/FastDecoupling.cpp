#include "FastDecoupling.h"

#include <cmath>
#include <cstdlib>
#include <ctime>

#include <algorithm>
#include <limits>
#include <numeric>
#include <vector>

#define CHRONO
#ifdef CHRONO
	#include "chrono.h"
#endif

/**
 * @file Implementation of the FastDecoupling class.
 */


// Include FFT3D support header
#include "fft_3D.h"

/**
 * Constructor.
 * @param imageFlash Image taken using flash.
 * Its values must be in the range [0,1].
 * @param imageNoFlash Imagen without flash, same conditions as the flash one.
 *
 * @ToDo: Check if they must be in Luv format.
 */
FastDecoupling::FastDecoupling(const CImg<>& imageFlash, const CImg<>& imageNoFlash)
	// Call base class constructor
	:Decoupling(imageFlash,imageNoFlash){
}

/**
 * Destructor.
 */
FastDecoupling::~FastDecoupling(){
	// Empty
}

/**
 * Clamp
 * Replace this code with something else...
 */
 template<typename Real,typename RealDummy1,typename RealDummy2>
 Real
FastDecoupling::clamp(const RealDummy1 min_value, const RealDummy2 max_value,
		    const Real x)const{

    return std::max(std::min(x,static_cast<Real>(max_value)),static_cast<Real>(min_value));
}

/**
 * Trilinear interpolation.
 *
 * Adapted from the code of Sylvain Paris and Francois Sillion,
 * found on the bilateral filter implementation.
 * @param array Input image (3 dimensions).
 * @param x X Coordinate.
 * @param y Y Coordinate.
 * @param z Z Coordinate.
 * @ToDo This should be placed in the Helper class
 */
float FastDecoupling::trilinearInterpolation(const CImg<>& array, const float x,
	const float y, const float z)const{


    const unsigned x_size = array.dimx();
    const unsigned y_size = array.dimy();
    const unsigned z_size = array.dimz();

    const unsigned x_index  = clamp(0,x_size-1,static_cast<unsigned>(x));
    const unsigned xx_index = clamp(0,x_size-1,x_index+1);

    const unsigned y_index  = clamp(0,y_size-1,static_cast<unsigned>(y));
    const unsigned yy_index = clamp(0,y_size-1,y_index+1);

    const unsigned z_index  = clamp(0,z_size-1,static_cast<unsigned>(z));
    const unsigned zz_index = clamp(0,z_size-1,z_index+1);

    const float x_alpha = x - x_index;
    const float y_alpha = y - y_index;
    const float z_alpha = z - z_index;

    return
      (1.0f-x_alpha) * (1.0f-y_alpha) * (1.0f-z_alpha) * array(x_index, y_index, z_index) +
      x_alpha        * (1.0f-y_alpha) * (1.0f-z_alpha) * array(xx_index,y_index, z_index) +
      (1.0f-x_alpha) * y_alpha        * (1.0f-z_alpha) * array(x_index, yy_index,z_index) +
      x_alpha        * y_alpha        * (1.0f-z_alpha) * array(xx_index,yy_index,z_index) +
      (1.0f-x_alpha) * (1.0f-y_alpha) * z_alpha        * array(x_index, y_index, zz_index) +
      x_alpha        * (1.0f-y_alpha) * z_alpha        * array(xx_index,y_index, zz_index) +
      (1.0f-x_alpha) * y_alpha        * z_alpha        * array(x_index, yy_index,zz_index) +
      x_alpha        * y_alpha        * z_alpha        * array(xx_index,yy_index,zz_index);
}

/**
 * Computes the bilateral filter using a signal processing approach.
 *
 * This bilateral filter is
 * @param I Image intensity layer.
 * @return J image, Large Scale layer.
 *
 * @note If this doesn't work, check that range_sampling and
 * space_sampling has been correctly replaced by the corresponding
 * sigmas (F/G).
 */
CImg<>& FastDecoupling::bilateralFilter(const CImg<>& I)const{
	CImg<float>* J = new CImg<float>(I,false);

	const unsigned width  = I.dimx();
	const unsigned height = I.dimy();

	/**
	 * @note VERY IMPORTANT:
	 * Maybe the sigmaF and sigmaG parameters should replace
	 * these sigma_r and sigma_s, instead of range_sigma
	 * and space_sigma.
	 */
	const float sigma_s = _sigmaF / _sigmaF;
	const float sigma_r = _sigmaG / _sigmaG;
	/*
	 * Previous two lines in the Paris and Durand code were:
	 * const real_type sigma_r = range_sigma / range_sampling;
	 * const real_type sigma_s = space_sigma / space_sampling;
	 */

	const unsigned padding_xy = static_cast<unsigned>(2.0 * sigma_s) + 1;
	const unsigned padding_z  = static_cast<unsigned>(2.0 * sigma_r) + 1;

	// Image statistics (for min and max)
	const CImgStats stats(I);
	// Maximum and min pixel of the image
	const float input_max	= stats.min;
	const float input_min	= stats.min;
	const float input_delta = input_max - input_min;

	const unsigned small_width  = static_cast<unsigned>((width  - 1) / _sigmaF) + 1 + 2*padding_xy;
	const unsigned small_height = static_cast<unsigned>((height - 1) / _sigmaF) + 1 + 2*padding_xy;
	const unsigned small_depth  = static_cast<unsigned>(input_delta / _sigmaG) + 1 + 2*padding_z;

	cerr << "Initialization of fft_suport...";
	FFT::Support_3D fft_support(small_width,small_height,small_depth);
	cerr << "done." << endl;

#ifdef CHRONO
	Chrono chrono("filter");
	chrono.start();

	Chrono chrono_down("downsampling");
	chrono_down.start();
#endif
	cerr << "Filling...";
	CImg<> w(small_width,small_height,small_depth);
	w.fill(0.0);
	CImg<> iw(small_width,small_height,small_depth);
	iw.fill(0.0);
cerr << "done."<<endl<<"First 2 FOR loop...";
	for(unsigned y=0,y_end=height;y<y_end;y++){
		for(unsigned x=0,x_end=width;x<x_end;x++){
			const unsigned small_x = static_cast<unsigned>
				(1.0*x / _sigmaF + 0.5) + padding_xy;
			const unsigned small_y = static_cast<unsigned>
				(1.0*y / _sigmaF + 0.5) + padding_xy;
			const unsigned small_z = static_cast<unsigned>
				((I(x,y)-input_min) / _sigmaG + 0.5) + padding_z;

			w(small_x,small_y,small_z)  += 1.0;
			iw(small_x,small_y,small_z) += I(x,y);

		} // for x
	} // for y
cerr << "done."<<endl;
	CImg<> kernel(small_width,small_height,small_depth);

	const unsigned half_width  = small_width/2;
    const unsigned half_height = small_height/2;
    const unsigned half_depth  = small_depth/2;
cerr << "3 FOR loop...";

	/*
	 * Optimization note:
	 * The original linear bilateral filter implementation
	 * loops for x, for y, for z, because the Array class
	 * is optimized for this kind of access.
	 * CImg was designed the other way around.
	 */
	// For each Z component...
	for(unsigned z=0,z_end=small_depth;z<z_end;z++){
		const float Z = static_cast<float>(z) - ((z>half_depth) ? small_depth : 0.0);

		// For each Y component...
		for(unsigned y=0,y_end=small_height;y<y_end;y++){
			const float Y = static_cast<float>(y) - ((y>half_height) ? small_height : 0.0);
			// For each X component...
			for(unsigned x=0,x_end=small_width;x<x_end;x++){
				const float X = static_cast<float>(x) - ((x>half_width) ? small_width : 0.0);

				// Here's the good stuff I think
				const float rr = (X*X + Y*Y) / (sigma_s*sigma_s) + Z*Z / (sigma_r*sigma_r);

				kernel(x,y,z) = std::exp(-rr*0.5);
			} // for x
		} // for y
	} // for z
cerr << "done." << endl;
#ifdef CHRONO
	chrono_down.stop();

	Chrono chrono_convolution("convolution");
	chrono_convolution.start();
#endif
	FFT::convolve_3D(iw,kernel,&iw,fft_support);
	FFT::convolve_3D(w,kernel,&w,fft_support);

#ifdef CHRONO
	chrono_convolution.stop();

	Chrono chrono_nonlinearities("nonlinearities");
	chrono_nonlinearities.start();
#endif

	for(unsigned y=0,y_end=height;y<y_end;y++){
		for(unsigned x=0,x_end=width;x<x_end;x++){
			const float z = I(x,y) - input_min;

			const float IW = trilinearInterpolation(iw,
							 static_cast<float>(x)/_sigmaF + padding_xy,
							 static_cast<float>(y)/_sigmaF + padding_xy,
							 z/_sigmaG + padding_z);

			const float W = trilinearInterpolation(w,
							static_cast<float>(x)/_sigmaF + padding_xy,
							static_cast<float>(y)/_sigmaF + padding_xy,
							z/_sigmaG + padding_z);

			(*J)(x,y) = IW / W;

		} // for x
	} // for y

#ifdef CHRONO
	chrono_nonlinearities.stop();
	chrono.stop();

	cout<<chrono.report()<<endl;
	cout<<chrono_down.report()<<endl;
	cout<<chrono_convolution.report()<<endl;
	cout<<chrono_nonlinearities.report()<<endl;
#endif

	return *J;
}

/**
 * Computes the fast bilateral,excluding pixels within shadow region.
 * @param I Image intensity layer
 * @param shadow Shadow bit map.
 * @return J image, Large Scale layer not affected by the shadow.
 */
CImg<>& FastDecoupling::bilateralFilter(const CImg<>& I, const CImg<bool>& shadow)const{
	CImg<>* J = new CImg<>(I,false);

	const unsigned width  = I.dimx();
	const unsigned height = I.dimy();

	/**
	 * @note VERY IMPORTANT:
	 * Maybe the sigmaF and sigmaG parameters should replace
	 * these sigma_r and sigma_s, instead of range_sigma
	 * and space_sigma.
	 */
	const float sigma_s = _sigmaF / _sigmaF;
	const float sigma_r = _sigmaG / _sigmaG;
	/*
	 * Previous two lines in the Paris and Durand code were:
	 * const real_type sigma_r = range_sigma / range_sampling;
	 * const real_type sigma_s = space_sigma / space_sampling;
	 */

	const unsigned padding_xy = static_cast<unsigned>(2.0 * sigma_s) + 1;
	const unsigned padding_z  = static_cast<unsigned>(2.0 * sigma_r) + 1;

	// Image statistics (for min and max)
	const CImgStats stats(I);
	// Maximum and min pixel of the image
	const float input_max	= stats.min;
	const float input_min	= stats.min;
	const float input_delta = input_max - input_min;

	const unsigned small_width  = static_cast<unsigned>((width  - 1) / _sigmaF) + 1 + 2*padding_xy;
	const unsigned small_height = static_cast<unsigned>((height - 1) / _sigmaF) + 1 + 2*padding_xy;
	const unsigned small_depth  = static_cast<unsigned>(input_delta / _sigmaG) + 1 + 2*padding_z;

	cerr << "Initialization of fft_suport...";
	FFT::Support_3D fft_support(small_width,small_height,small_depth);
	cerr << "done." << endl;

#ifdef CHRONO
	Chrono chrono("filter");
	chrono.start();

	Chrono chrono_down("downsampling");
	chrono_down.start();
#endif
	cerr << "Filling...";
	CImg<> w(small_width,small_height,small_depth);
	w.fill(0.0);
	CImg<> iw(small_width,small_height,small_depth);
	iw.fill(0.0);
	CImg<> sw(small_width,small_height,small_depth);
	sw.fill(false);

cerr << "done."<<endl<<"First 2 FOR loop...";
	for(unsigned y=0,y_end=height;y<y_end;y++){
		for(unsigned x=0,x_end=width;x<x_end;x++){
			const unsigned small_x = static_cast<unsigned>
				(1.0*x / _sigmaF + 0.5) + padding_xy;
			const unsigned small_y = static_cast<unsigned>
				(1.0*y / _sigmaF + 0.5) + padding_xy;
			const unsigned small_z = static_cast<unsigned>
				((I(x,y)-input_min) / _sigmaG + 0.5) + padding_z;

			w(small_x,small_y,small_z)  += 1.0;
			sw(small_x,small_y,small_z) += !shadow(x,y);
			iw(small_x,small_y,small_z) += I(x,y);

		} // for x
	} // for y
cerr << "done."<<endl;
	CImg<> kernel(small_width,small_height,small_depth);

	const unsigned half_width  = small_width/2;
    const unsigned half_height = small_height/2;
    const unsigned half_depth  = small_depth/2;
cerr << "3 FOR loop...";

	/*
	 * Optimization note:
	 * The original linear bilateral filter implementation
	 * loops for x, for y, for z, because the Array class
	 * is optimized for this kind of access.
	 * CImg was designed the other way around.
	 */
	// For each Z component...
	for(unsigned z=0,z_end=small_depth;z<z_end;z++){
		const float Z = static_cast<float>(z) - ((z>half_depth) ? small_depth : 0.0);

		// For each Y component...
		for(unsigned y=0,y_end=small_height;y<y_end;y++){
			const float Y = static_cast<float>(y) - ((y>half_height) ? small_height : 0.0);
			// For each X component...
			for(unsigned x=0,x_end=small_width;x<x_end;x++){
				const float X = static_cast<float>(x) - ((x>half_width) ? small_width : 0.0);

				// Here's the good stuff I think
				const float rr = (X*X + Y*Y) / (sigma_s*sigma_s) + Z*Z / (sigma_r*sigma_r);

				kernel(x,y,z) = std::exp(-rr*0.5);
			} // for x
		} // for y
	} // for z
cerr << "done." << endl;
#ifdef CHRONO
	chrono_down.stop();

	Chrono chrono_convolution("convolution");
	chrono_convolution.start();
#endif
	FFT::convolve_3D(iw,kernel,&iw,fft_support);
	FFT::convolve_3D(sw,kernel,&sw,fft_support);
	FFT::convolve_3D(w,kernel,&w,fft_support);

#ifdef CHRONO
	chrono_convolution.stop();

	Chrono chrono_nonlinearities("nonlinearities");
	chrono_nonlinearities.start();
#endif

	for(unsigned y=0,y_end=height;y<y_end;y++){
		for(unsigned x=0,x_end=width;x<x_end;x++){
			const float z = I(x,y) - input_min;

			const float IW = trilinearInterpolation(iw,
							 static_cast<float>(x)/_sigmaF + padding_xy,
							 static_cast<float>(y)/_sigmaF + padding_xy,
							 z/_sigmaG + padding_z);

			const float W = trilinearInterpolation(w,
							static_cast<float>(x)/_sigmaF + padding_xy,
							static_cast<float>(y)/_sigmaF + padding_xy,
							z/_sigmaG + padding_z);

			(*J)(x,y) = IW / W;

		} // for x
	} // for y

#ifdef CHRONO
	chrono_nonlinearities.stop();
	chrono.stop();

	cout<<chrono.report()<<endl;
	cout<<chrono_down.report()<<endl;
	cout<<chrono_convolution.report()<<endl;
	cout<<chrono_nonlinearities.report()<<endl;
#endif

	return *J;
}

/**
 * Computes the Large Scale layer using Cross Bilateral filtering,
 * for poor-lighted images.
 * Bilateral Filtering, para situaciones con muy poca iluminacion
 * @param iNF Non-flash image intensity layer.
 * @param iF Flash image intensity layer.
 * @note This filter implies that shadows cannot be corrected.
 * @return J image, Large Scale.
 */
CImg<float>& FastDecoupling::crossBilateralFilter(const CImg<>& iNF,const CImg<>& iF)const{
	// La imagen J que devolvemos
	CImg<float>* J = new CImg<float>(iNF,false);

	return *J;
}
