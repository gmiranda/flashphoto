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

//! Standard i/o
#include <iostream>
//! Use console output
using std::cout;
//! And new line
using std::endl;

//! auto_ptr
#include <memory>

//! CImg Library
#include "CImg.h"
//! Use CImg namespace
using namespace cimg_library;

//! Include Decoupling class
#include "Decoupling.h"

//! Include Helper functions
#include "Helper.h"

//! Shadow treatment class
#include "Shadow.h"


/// Definicion del nombre del archivo de la imagen con flash
#define FLASH_FILE argv[2]
/// El de la imagen sin flash
#define NOFLASH_FILE argv[3]
/// Opciones para la reconstruccion
#define OPTIONS *argv[1]

/// Opciones de reconstruccion posibles
typedef enum {CROSS='c',DETAILS_CORRECTION='d',NORMAL='n'};

void printUsage();

/**
 * This project focuses on implementing flash photography related algorithms.
 * The first algorithm to be implemented is called "Flash Photography
 * Enhancement via Intrinsic Relighting", presented in SIGGRAPH 2004
 *
 * Given one photograph, using the flash of the camera, and another
 * without the flash, the result is an image using the details presents
 * in a flash photo, and the colors and ambience of the non flash photo
 * (that also means no red eyes and no shadows because of the flash).
 
 * @author Guillermo Miranda Alamo <gmiranda@teleline.es>
 * @date July 2005 - September 2006
 * @param argc Number of arguments.
 * @param argv Arguments
 * @version 4.0 Experimental
 * @see http://artis.imag.fr/Publications/2004/ED04/
 */
int main(int argc, char* argv[]){
	// Mensaje de bienvenida
	cout << "Flash photography enhancement via intrinsic relighting" << endl << endl;
	cout << "Implementation by Guillermo Miranda Alamo - gmiranda@teleline.es" << endl << endl;

	// Comprobamos el nº de argumentos
	if(argc<4){
		// If there aren't enough parameters, print syntax
		cerr << "[ERROR] This program requires at least two source images and you must choose an option" << endl;
		printUsage();
		
		return -1;
	}
	// If the user chose an invalid option, print syntax
	if((OPTIONS!=CROSS)&&(OPTIONS!=DETAILS_CORRECTION)&&(OPTIONS!=NORMAL)){
		cerr << "[ERROR] No valid option selected." << endl;
		printUsage();
		
		return -2;
	}

	// Cargamos la imagen con flash
	CImg<> image(FLASH_FILE);
	image/=255.0f;


	// Ahora la que esta hecha sin flash
	CImg<> imageNoFlash(NOFLASH_FILE);
	imageNoFlash/=255.0f;


	// Preparamos Image Decoupling a partir de la imagen "image"
	// Autopointer (it will be destroyed after function exit), use experimental algorithms
	std::auto_ptr<Decoupling> deco(new DecouplingExperimental(image,imageNoFlash));
	
	image.display("Fotografia con flash original");
	imageNoFlash.display("Fotografia sin flash original");

	// Obtenemos la intensidad
	CImg<float> inten=deco->getIntensity(image);
	//(inten*255.0f).display("inten");
	//CImg<> ls = deco->piecewiseBilateralFilter(inten*255.0f);
	//ls.display("Piecewise");
	// Sumamos para no tener un menos infinito al calcular el logaritmo
	// Tambien se podria hacer en la imagen directamente
	inten+=Helper::fakezero;

	// Intensidad de la sin flash
	CImg<float> intenNF =deco->getIntensity(imageNoFlash);
	intenNF+=Helper::fakezero;

	// PRUEBAS






	// FIN DE PRUEBAS

	// Deteccion de sombras
	CImg<bool> shadow;
	// Solo se detectan y corrigen si no hacemos cross bilateral
	if(OPTIONS!=CROSS)
		shadow=Shadow::shadow(inten,intenNF);

	cout << "Computing Large Scale layer..." << endl;
	
	CImg<> largeScale;
	// Si hacemos no cross bilateral filter
	if(OPTIONS!=CROSS){
		// Utilizamos la sombra para calcular la large scale
		// Descomenta la siguiente linea para cargar de archivo y comenta la otra
		//largeScale=CImg<>("largeScaleF.bmp");largeScale/=255.0f;
		largeScale= deco->bilateralFilter(inten,shadow);
		//largeScale.display("Large scale");
	}
	// Si lo hacemos
	else{
		// No podemos utilizar la sombra.
		largeScale= deco->bilateralFilter(inten);
		//largeScale= deco->bilateralFilterAlt(inten);
		//largeScale.display("Large Scale");
	}
	(largeScale*255.0f).save("largeScaleF.bmp");
	//largeScale.display("Large Scale Flash");


	CImg<float> color = deco->getColor(image.get_log10(),inten.get_log10());


	// Correccion de la capa de color
	if(OPTIONS!=CROSS){
		cout << "Computing Color layer..." << endl;
		
		CImg<float> colorNF = deco->getColor(imageNoFlash.get_log10(),intenNF.get_log10());
		color.pow10();colorNF.pow10();	// Convertimos de dominio log10 a normal
		CImg<> colorCor=ShadowExperimental::colorCorrection(colorNF,color,shadow);
		// Hay que normalizar para devolver los valores a su rango
		colorCor.normalize(0.0f,1.0f);
		(colorCor*255.0f).save("colorcor.bmp");
		// Descomenta esta linea para cargar la correccion de color,
		//y comenta hasta Shadow::colorCorrection()
		//CImg<> colorCor("colorcor.bmp");colorCor/=255.0f;
		colorCor.display("correccion color");
		color=colorCor;
		color.log10();colorNF.log10();
	}

	// Hago la diferencia de inten y largeScale
	CImg<float> details= (inten.get_log10()-largeScale.get_log10());

	cout << "Computing Details layer..." << endl;
	
	CImg<float> largeScaleNF;
	// Si se quiere hacer filtro bilateral cruzado
	if(OPTIONS==CROSS){
		// Pues se hace!
		largeScaleNF= deco->crossBilateralFilter(intenNF,inten);
		// Comenta la de arriba si cargas de archivo, y descomenta la de abajo
		//largeScaleNF=CImg<>("cakeNo-flashsLS_diag.bmp");
	}
	// Sino, se hace el normal
	else{
		// Descomenta la siguiente linea para cargar de archivo y comenta la otra
		//largeScaleNF=CImg<>("largeScaleNF.bmp");largeScaleNF/=255.0f;
		largeScaleNF= deco->bilateralFilter(intenNF);
	}
	(largeScaleNF*255.0f).save("largeScaleNF.bmp");
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

	cout << "Reconstrucing..." << endl;
	
	CImg<> recons = deco->reconstruct(color,details,largeScaleNF);
	recons.display("Reconstruccion");
	// Preparamos para guardar como enteros
	recons*=255.0f;
	// Guardamos
	recons.save("reconstruccion.bmp");

	return 0;
}

/**
 * Prints the syntax (usage) of this program
 */
void printUsage(){
	cerr << "Usage:"<<endl<<" flash [options] [flashImage] [noFlashImage]" << endl;
	cerr << "(without the brackets)" << endl;
	cerr << "The options must be ONE of these:" << endl;
	cerr << "c    Use cross bilateral filter (shadows won't be corrected)" << endl;
	cerr << "d    Use details correction - Use this for the best results" << endl;
	cerr << "n    Normal mode: detect shadows, but don't correct the details and color layer. " << endl;
}
