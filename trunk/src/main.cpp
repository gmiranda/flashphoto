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

// Libreria de CImg
#include "CImg/CImg.h"
/// Usamos su namespace
using namespace cimg_library;

// Mi clase de decoupling
#include "Decoupling.h"

// Helper
#include "Helper.h"

// Para deteccion de sombras
#include "Shadow.h"

/// Usamos cout
using std::cout;
/// Y tambien usamos end
using std::endl;

/// Definicion del nombre del archivo de la imagen con flash
#define FLASH_FILE argv[2]
/// El de la imagen sin flash
#define NOFLASH_FILE argv[3]
/// Opciones para la reconstruccion
#define OPTIONS *argv[1]

/// Opciones de reconstruccion posibles
typedef enum {CROSS='c',DETAILS_CORRECTION='d',NORMAL='n'};

/**
 * Rutina principal del programa.
 * @author Guillermo Miranda Alamo <gmiranda@teleline.es>
 * @date July-September 2005
 * @param argc Num. de argumentos.
 * @param argv Argumentos.
 * @version 3.2.
 */
int main(int argc, char* argv[]){
	// Mensaje de bienvenida
	cout << "Flash photography enhancement via intrinsic relighting" << endl << endl;
	cout << "Implementation by Guillermo Miranda Alamo - gmiranda@teleline.es" << endl << endl;

	// Comprobamos el nº de argumentos
	if(argc<4){
		cerr << "[ERROR] This program requires at least two source images and you must choose an option" << endl;
		cerr << "Usage:"<<endl<<" flash [options] [flashImage] [noFlashImage]" << endl;
		cerr << "(without the brackets)" << endl;
		cerr << "The options must be ONE of these:" << endl;
		cerr << "c    Use cross bilateral filter (shadows won't be corrected)" << endl;
		cerr << "d    Use details correction" << endl;
		cerr << "n    Normal mode: detect shadows, but don't correct the details and color layer. "
			<< "Use this for the best results"<< endl;
		return -1;
	}
	// Si se ha elegido una opcion invalida
	if((OPTIONS!=CROSS)&&(OPTIONS!=DETAILS_CORRECTION)&&(OPTIONS!=NORMAL)){
		cerr << "[ERROR] No valid option selected." << endl;
		cerr << "Usage:"<<endl<<" flash [options] [flashImage] [noFlashImage]" << endl;
		cerr << "(without the brackets)" << endl;
		cerr << "The options must be ONE of these:" << endl;
		cerr << "c    Use cross bilateral filter (shadows won't be corrected)" << endl;
		cerr << "d    Use details correction" << endl;
		cerr << "n    Normal mode: detect shadows, but don't correct the details and color layer. "
			<< "Use this for the best results"<< endl;
		return -2;
	}

	// Cargamos la imagen con flash
	CImg<> image(FLASH_FILE);
	image/=255.0f;


	// Ahora la que esta hecha sin flash
	CImg<> imageNoFlash(NOFLASH_FILE);
	imageNoFlash/=255.0f;


	// Preparamos Image Decoupling a partir de la imagen "image"
	Decoupling deco(image,imageNoFlash);
	image.display("Fotografia con flash original");
	imageNoFlash.display("Fotografia sin flash original");

	// Obtenemos la intensidad
	CImg<float> inten=deco.getIntensity(image);
	(inten*255.0f).display("inten");
	//CImg<> ls = deco.piecewiseBilateralFilter(inten*255.0f);
	//ls.display("Piecewise");
	// Sumamos para no tener un menos infinito al calcular el logaritmo
	// Tambien se podria hacer en la imagen directamente
	inten+=0.001f;

	// Intensidad de la sin flash
	CImg<float> intenNF =deco.getIntensity(imageNoFlash);
	intenNF+=0.001f;

	// PRUEBAS
	CImg<> kernelG=Decoupling::gaussianKernel(5);
	kernelG.display();
	kernelG.resize(inten,0);
	kernelG.display("kernelG redimensionada");
	CImgl<> kernelFFT = Helper::getFFT(kernelG);

	//inten.get_convolve(kernelG).display();

	CImgl<> intenFFT = Helper::getFFT(inten);
	cimgl_map(intenFFT,l) intenFFT[l].mul(kernelFFT[l]);
	//CImgl<> intenFFTi = (intenFFT[0].mul(kernelFFT[0])+intenFFT[1].mul(kernelFFT[1])).get_FFT(true);
	CImgl<> intenFFTi = intenFFT.get_FFT(true);
	intenFFTi[0].display("ffti 0");
	intenFFTi[1].display("ffti 1");
	cout << "Suma" << endl;
	CImg<> inteni=(intenFFTi[0]/*+intenFFTi[1]*/).crop(0,0,320,240);
	//CImg<> inteni=Helper::getiFFT(intenFFT,inten.dimx(),inten.dimy());
	cout << "Crop" << endl;
	inteni.display("intensidad suavizada ifft");





	// FIN DE PRUEBAS

	// Deteccion de sombras
	CImg<bool> shadow;
	// Solo se detectan y corrigen si no hacemos cross bilateral
	if(OPTIONS!=CROSS)
		shadow=Shadow::shadow(inten,intenNF);


	CImg<> largeScale;
	// Si hacemos no cross bilateral filter
	if(OPTIONS!=CROSS){
		// Utilizamos la sombra para calcular la large scale
		// Descomenta la siguiente linea para cargar de archivo y comenta la otra
		//largeScale=CImg<>("largeScaleF.png");
		largeScale= deco.bilateralFilter(inten,shadow);
	}
	// Si lo hacemos
	else{
		// No podemos utilizar la sombra.
		largeScale= deco.piecewiseBilateralFilter(inten);
		//largeScale= deco.bilateralFilterAlt(inten);
		largeScale.display("Large Scale");
	}
	//largeScale/=255.0f;	// Descomenta si cargas de archivo
	(largeScale*255.0f).save("largeScaleF.png");
	//largeScale.display("Large Scale Flash");


	CImg<float> color = deco.getColor(image.get_log10(),inten.get_log10());

	// Correccion de la capa de color
	if(OPTIONS!=CROSS){
		CImg<float> colorNF = deco.getColor(imageNoFlash.get_log10(),intenNF.get_log10());
		color.pow10();colorNF.pow10();	// Convertimos de dominio log10 a normal
		CImg<> colorCor=Shadow::colorCorrection(colorNF,color,shadow);
		// Hay que normalizar para devolver los valores a su rango
		colorCor.normalize(0.0f,1.0f);
		(colorCor*255.0f).save("colorcor.png");
		// Descomenta esta linea para cargar la correccion de color,
		//y comenta hasta Shadow::colorCorrection()
		//CImg<> colorCor("colorcor.png");colorCor/=255.0f;
		colorCor.display("correccion color");
		color=colorCor;
		color.log10();colorNF.log10();
	}

	// Hago la diferencia de inten y largeScale
	CImg<float> details= (inten.get_log10()-largeScale.get_log10());


	CImg<float> largeScaleNF;
	// Si se quiere hacer filtro bilateral cruzado
	if(OPTIONS==CROSS){
		// Pues se hace!
		largeScaleNF= deco.crossBilateralFilterAlt(intenNF,inten);
		// Comenta la de arriba si cargas de archivo, y descomenta la de abajo
		//largeScaleNF=CImg<>("cakeNo-flashsLS_diag.bmp");
	}
	// Sino, se hace el normal
	else{
		// Descomenta la siguiente linea para cargar de archivo y comenta la otra
		//largeScaleNF=CImg<>("largeScaleNF.png");
		largeScaleNF= deco.bilateralFilterAlt(intenNF);
	}
	//largeScaleNF/=255.0f;	// Descomenta esto si cargas de archivo
	(largeScaleNF*255.0f).save("largeScaleNF.png");
	//largeScaleNF.display("Large Scale no flash");
	// Convertimos la capa largeScale a log10
	largeScaleNF.log10();

	// Si se quiere que se corrija la capa de detalles
	if(OPTIONS==DETAILS_CORRECTION){
		// Cojo los detalles de la imagen sin flash.
		CImg<> detailsNF = (intenNF.get_log10()-largeScaleNF);
		// Hacemos la correccion, pero no se puede hacer en dominio log10.
		details = Shadow::detailsCorrection(details.pow10(),detailsNF.pow10(),shadow).log10();
	}


	CImg<> recons = deco.reconstruct(color,details,largeScaleNF);
	recons.display("Reconstruccion");
	// Preparamos para guardar como enteros
	recons*=255.0f;
	// Guardamos
	recons.save("reconstruccion.png");

	return 0;
}
