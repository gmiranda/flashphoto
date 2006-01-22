#include "Shadow.h"

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

// Deteccion de umbra

/**
 * Busca en el histograma de la imagen deltaI el primer minimo
 * local menor que 0.2.
 * @param histo histograma (128 bins, suavizado gaussiano a 2.0)
 * diferencia entre las intensidades de la imagen con flash
 * y la imagen sin flash.
 * @return valor minimo menor que 0.2.
 * @ToDo que funcione bien.
 */
float Shadow::umbraCandidate(const CImg<float>& histo){
	// El minimo encontrado hasta ahora
	float min = 127*0.2f;
	// Obtenemos su puntero de datos
	const float* ptr = histo.ptr();
	
	/*
	 * Recorremos el histograma.
	 * Recuerda que la i nos dice el valor del pixel,
	 * y en el histograma este los valores estan entre 0 y 127,
	 * por lo que para encontrar un valor de pixel < 0.2, hay que
	 * multiplicar 0.2 por 127.
	 */
	for(unsigned int i=1;i<127*0.2f;i++){
		// Si hay pixeles con ese valor y es menor que 0.2f, nos lo quedamos
		if((ptr[i]>0)&&(i<127*0.2f))
			min=i;
	}
	// Y ahora dividimos entre 0.2 para devolver un valor entero
	return (float)min/127.0f;
}

/**
 * Descarta los pixeles cuyo valor sea superior al limite
 * deseado.
 * @param image imagen sobre la que trabajar.
 * @param threshold umbral, limite que no se quiere sobrepasar.
 * @return nueva imagen booleana con los pixeles no descartados, que
 * seran los pertenecientes a la umbra.
 */
CImg<bool>& Shadow::dischardPixels(const CImg<>& image, const float threshold){
	// Creamos una imagen del mismo tamaño que image
	// pero sin copiar los valores de los pixeles
	CImg<bool>* res = new CImg<bool>(image,false);
	
	// Obtenemos su puntero de datos
	const float* sourceData = image.ptr();
	bool* newData = res->ptr();
	
	// Recorremos los pixeles
	for(unsigned int i=0;i<image.size();i++){
		// Los pixeles > threshold los ponemos a 1.0f para verlos.
		newData[i]=(sourceData[i]>threshold)?false:true;
	}
	
	return *res;	
}

/**
 * Calcula los pixeles correspondientes a la umbra
 * @param intenF Intensidad de la imagen con flash.
 * @param intenNF Intensidad de la imagen sin flash.
 * @return Mapa binario con la umbra.
 */
CImg<bool>& Shadow::umbra(const CImg<>& intenF, const CImg<>& intenNF){
	// Creamos una nueva imagen con las dimensiones de intenF
	CImg<bool>* umbra=new CImg<bool>(intenF,false);
	
	// Calculamos la diferencia de intensidades
	CImg<> dI = (intenF-intenNF);
	// Ahora el histograma, con 128 bins
	CImg<> histo = dI.get_histogram(128,0.0f,1.0f);
	//histo.display("histograma 128");
	// Suavizamos el histograma
	histo.blur(2.0f);
	
	// Buscamos el minimo local menor que 0.2
	float min = umbraCandidate(histo);
	
	//cerr << "Minimo="<<min<<endl;
	
	(*umbra) = dischardPixels(dI,min);
	// Multiplicamos la delta por el mapa de umbra

	return *umbra;
}

/**
 * Calcula la penumbra de las sombras de la imagen.
 * @param intenF Intensidad de la imagen con flash.
 * @param intenNF Intensidad de la imagen sin flash.
 * @param umbra mapa binario de la umbra.
 * @return mapa binario con la penumbra.
 */
CImg<bool> Shadow::penumbra(const CImg<>& intenF, const CImg<>& intenNF, const CImg<bool>& umbra){
	/// Gradientes
	// Vamos a ver el gradiente de If
	CImg<> gF = Helper::get_magnitude_gradient(intenF); 
	// Ahora suavizamos
	gF.blur(2.0f);
	// Gradiente de Inf
	CImg<> gNF = Helper::get_magnitude_gradient(intenNF);
	gNF.blur(2.0f);

	// Vamos a ver que pixeles son mas fuertes en la imagen con flash
	// Truncando conseguimos eliminar pixeles con gradiente mas fuerte
	// en la imagen sin flash,
	CImg<bool> gDif(Helper::getTruncate(gF-gNF));
	//CImg<float>(gDif).display("Diferencia de gradientes suavizados");

	
	/// Buscamos los pixeles de la penumbra cerca de la umbra
	// Ahora habria que mirar, para esos pixeles, sus vecinos
	// Creamos el box filter
	CImg<> boxFilter = Helper::boxFilter();
	//boxFilter.display("Box filter mask");
	
	// Convolvemos el mapa binario de umbra con el box filter
	CImg<> mapConvolved = umbra.get_convolve(boxFilter);
	//mapConvolved.display("Mapa binario de umbra convolved");
	// Ahora ya tenemos los pixeles de penumbra donde hay vecinos en la umbra


	/// Buscamos la zona de penumbra para los trozos pequeños
	// Filtro tipo box para objetos pequeños
	// segun el 0.7% de la diagonal
	CImg<> boxFilterTiny = Helper::boxFilter(intenNF,0.007f);
	//boxFilterTiny.display("Box filter para objetos pequeños");
	
	// La convolucion
	CImg<> mapConvolvedTiny = ((CImg<float>)gDif).get_convolve(boxFilterTiny);
	//mapConvolvedTiny.display("Convolucion entre umbra y box tiny");
	
	// Nos quedamos solo con los pixeles con al menos el 80%
	// de sus vecinos en la penumbra
	mapConvolvedTiny=dischardPixels(mapConvolvedTiny,0.8f);

	
	/*
	 * Devolvemos los pixeles con mayor gradiente en la imagen flash
	 * y que tienen 1 vecino en la umbra, o los pixeles que solo
	 * tienen penumbra, que son objetos muy pequeños generalmente.
	 *
	 * Se suman los resultados del calculos de pixeles umbra vecinos,
	 * y nos quedamos con los pixeles con gradiente mayor en la imagen
	 * con flash que cumplan una de las dos condiciones.
	 */
	return mapConvolved+mapConvolvedTiny;
}

/**
 * Calcula la sombra de la imagen (umbra y penumbra) provocada por el flash.
 * @param intenF Intensidad de la imagen con flash.
 * @param intenNF Intensidad de la imagen sin flash.
 * @return Mapa binario con la sombra de la imagen.
 */
CImg<bool>& Shadow::shadow(const CImg<>& intenF, const CImg<>& intenNF){
	// Calculamos la sombra en la penumbra
	CImg<bool>* sombra = new CImg<bool>(Shadow::umbra(intenF,intenNF));
	
	// Ahora la penumbra, a partir de la sombra que ya tenemos (umbra)
	CImg<bool> penumbra = Shadow::penumbra(intenF,intenNF,*sombra);
	// Sumamos la penumbra y ya esta la sombra
	// Nota: creo que la penumbra ya contiene todos los pixeles de la umbra
	(*sombra)+=penumbra;

	//intenF.get_mul(*sombra).display("If sombras");	
	
	// Devolvemos la sombra
	return *sombra;
}

/**
 * Realiza la correcion del color de la zona donde hay sombra.
 * @param noFlash foto sin flash.
 * @param flash foto con flash.
 * @param shadow mapa binario de la zona con sombra.
 * @return nueva imagen con flash, con los colores de los pixeles donde habia sombra corregidos.
 * @ToDo usar Luv en vez de Lab, si realmente es necesario.
 */
CImg<>& Shadow::colorCorrection(const CImg<>& noFlash,const CImg<>& flash,const CImg<bool>& shadow){
	// Imagen resultante, mismas dimensiones que flash, sin copiar pixeles
	CImg<>* color = new CImg<>(flash,false);
	
	// Las imagenes de color tienen que estar en Luv
	const CImg<> noFlashLuv = noFlash.get_RGBtoLab();
	const CImg<> flashLuv = flash.get_RGBtoLab();
	
	// Varianza f es el 2.5% de la diagonal de la imagen
	const float sigmaF = 0.025f*Helper::diagonalLength(flash);
	// La varianza f es 0.01
	const float sigmaG = 0.01f;
	
	// Para todas las Ys
	for(int y=0;y<color->dimy();y++){
		cerr << "\rY=" << y;
		// Para todas las Xs
		for(int x=0;x<color->dimx();x++){
			// La intensidad la copiamos
			//(*color)(x,y,0)=flashLuv(x,y,0);
			// Para las capas de color

			Pixel Is((float)x/color->dimx(),(float)y/color->dimy());
			// Calculamos 1/k(s) * SUM (...)
			// k(s) lo calculamos a la vez que el sumatorio
			float k=0;
			float sumL=0,sumU=0,sumV=0;
			// Vuelvo a recorrer todos los pixeles de la imagen
			// Las coordenadas x2,y2 corresponden al pixel p
			for(int y2=0;y2<color->dimy();y2++){
				for(int x2=0;x2<color->dimx();x2++){
					// Si el pixel esta en la sombra
					if(shadow(x2,y2))
						// No cuenta
						continue;
					
					// Pixel con el que comparar el actual
					Pixel Ip((float)x2/color->dimx(),(float)y2/color->dimy());
					// Producto de las gaussianas
					register float distancia = Helper::gaussianF(Ip.euclidean(Is),sigmaF);
					// Si la gaussiana de la distancia es 0
					if(distancia==0.0f)
						// No cuenta: saltamos, se acelera un poco
						continue;
					
					// Otra opcion: multiplicar las gaussianas de intensidad (no funciona muy mal asi)
					float prod =distancia*Helper::gaussianG(noFlashLuv(x2,y2,1)-noFlashLuv(x,y,1),sigmaG)*Helper::gaussianG(noFlashLuv(x2,y2,2)-noFlashLuv(x,y,2),sigmaG)*Helper::gaussianG(noFlashLuv(x2,y2,0)-noFlashLuv(x,y,0),sigmaG);

					// Si el producto es 0
					if(prod==0.0f)
						// No cuenta: saltamos, se acelera un poco
						continue;
					
					// Le sumamos el producto a K para la capa u
					k+=prod;

					// Y al sumatorio le sumamos el producto por la capa u
					sumU+=prod*flashLuv(x2,y2,1);
					// Y por la capa v
					sumV+=prod*flashLuv(x2,y2,2);
					// y por L
					sumL+=prod*flashLuv(x2,y2,0);
				}
			}
			// Descartamos k si esta por debajo de un umbral
			if(1.0f/std::log10(k)<0.02f){
				(*color)(x,y,1)=flashLuv(x,y,1);
				(*color)(x,y,2)=flashLuv(x,y,2);
				(*color)(x,y,0)=flashLuv(x,y,0);
				continue;
			}
			
			// Ahora ya tenemos k(s) y el sumatorio:
			// Para u...
			(*color)(x,y,1)=sumU/k;
			// Y para v...
			(*color)(x,y,2)=sumV/k;
			// ...y L
			(*color)(x,y,0)=sumL/k;

		}
	}
	cerr << std::endl;
	//color->LabtoRGB();
	return (*color).LabtoRGB();
}

/**
 * Corrige el ruido de la imagen resultante de la correcion de color,
 * combinando linealmente los pixeles de la capa de detalles de la imagen
 * con flash, y la capa de detalles de la imagen sin flash.
 * @note Se recomienda no hacer esta correccion con imagenes en log10.
 * @param detailsCorrected capa de detalles obtenida a partir de la imagen
 * con flash corregida.
 * @param detailsNoFlash capa de detalles obtenida de la imagen sin flash.
 * @param shadow mapa binario de sombras.
 * @return capa de detalles con la sombra corregida.
 */
CImg<>& Shadow::detailsCorrection(const CImg<>& detailsCorrected, const CImg<>& detailsNoFlash, const CImg<bool>& shadow){
	// Escalamos los detalles de la imagen no flash
	CImg<> detailsNFscaled(detailsNoFlash);
	// Image statistics (for min and max)
	const CImgStats stats(detailsCorrected); 
	
	// Adjust the details image
	detailsNFscaled.normalize(stats.min,stats.max);
	
	// La nueva imagen, no se le dan valores todavia
	CImg<>* details = new CImg<>(/*detailsCorrected,false*/detailsNFscaled);
	
	// Recorremos todos los pixeles
	/*for(int y=0;y<detailsCorrected.dimy();y++){
		for(int x=0;x<detailsCorrected.dimx();x++){
			//Si el pixel esta en la sombra,
			if(shadow(x,y)){
				//lo obtenemos de los detalles sin flash
				(*details)(x,y)=detailsNFscaled(x,y);
				// Siguiente!
				continue;
			}
			// Sino, es seguro coger el pixel de la imagen corregida
			(*details)(x,y)=detailsCorrected(x,y);
		}
}*/
	
	// Devolvemos la imagen resultante
	return *details;
}
