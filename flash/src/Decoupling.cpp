#include "Decoupling.h"

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

/*
 * Base Decoupling class implementation
 */

/**
 * Constructor.
 * @param imageFlash Image taken using flash.
 * Its values must be in the range [0,1].
 * @param imageNoFlash Imagen without flash, same conditions as the flash one.
 *
 * @ToDo: Check if they must be in Luv format.
 */
Decoupling::Decoupling(const CImg<>& imageFlash, const CImg<>& imageNoFlash)
	: _sigmaF(0.015f*Helper::diagonalLength(imageFlash)),_sigmaG(0.4f){
	// Construimos la imagen miembro(que es float)
	// a partir de la que nos pasan de parametro (ubyte)
		_imageFlash = imageFlash;

	// Lo mismo con la imagen sin flash
	_imageNoFlash = imageNoFlash;

}

/**
 * Destructor.
 */
Decoupling::~Decoupling(){
	// Empty
}

/**
 * Nos devuelve la intensidad.
 * @param image Imagen de la cual sacamos la intensidad.
 * @return Intensidad de la imagen (objeto nuevo, modificable).
 *
 * @ToDo: hacer inline.
 * @ToDo: comprobar si tiene que hacerse la copia (o quitar el &).
 */
CImg<float>& Decoupling::getIntensity(const CImg<float>& image)const{
	/**
	* Hay que crear una imagen a partir del canal 0
	* y devolverla.
	* Si solo devolviera el get_channel daria un error :)
	*/
	// Hacemos una copia de la imagen original
	CImg<float>* inten = new CImg<float>(image);

	for(int x=0;x<inten->dimx();x++){
		for(int y=0;y<inten->dimy();y++){
			// Por si 0/0_
			if((image(x,y,0)+image(x,y,1)+image(x,y,2))==0){
				// Ponemos el pixel a 0
				(*inten)(x,y)=0;
				// Pasamos al siguiente pixel
				continue;
			}
			(*inten)(x,y)=(image(x,y,0)/(image(x,y,0)+image(x,y,1)+image(x,y,2)))*image(x,y,0)+
					(image(x,y,1)/(image(x,y,0)+image(x,y,1)+image(x,y,2)))*image(x,y,1)+
					(image(x,y,2)/(image(x,y,0)+image(x,y,1)+image(x,y,2)))*image(x,y,2);
		}
	}

	//inten->RGBtoLuv();
	inten->channel(0);
	Helper::truncate(*inten,255,0);
	return *inten;
}

/**
 * Metodo que calcula la imagen con la informacion de color.
 * a partir de la imagen con la intensidad de la fotografia tomada
 * con flash.
 * @param intensity Imagen con la intensidad, calculada previamente
 * y en dominio log10.
 * @return Imagen con la informacion de color, en dominio log10.
 *
 * @ToDo: hacer inline.
 */
CImg<float>& Decoupling::getColor(const CImg<float>& intensity)const{
	return getColor(_imageFlash.get_log10(),intensity);
}

/**
 * Metodo que calcula la imagen con la informacion de color.
 * @param image Imagen de la que se quiere extraer la capa de color,
 * en dominio log10.
 * @param intensity Imagen con la intensidad, calculada previamente,
 * y en dominio log10.
 * @return Imagen con la informacion de color, en log10.
 *
 * @ToDo: hacer inline.
 */
CImg<float>& Decoupling::getColor(const CImg<>& image, const CImg<float>& intensity)const{

	CImg<float>* color = new CImg<float>(image);

	// La capa de color es la diferencia de la imagen
	// original y la intensidad en dominio log10
	color->ref_channel(0)=color->get_channel(0)-intensity;
	color->ref_channel(1)=color->get_channel(1)-intensity;
	color->ref_channel(2)=color->get_channel(2)-intensity;


	return *color;
}

/**
 * Reconstruye la imagen a partir de las imagenes descompuestas.
 * @param color Capa de color. Ha de estar en log10.
 * @param details Capa de detalles. Ha de estar en log10.
 * @param largeScaleNF capa large scale sin flash. Ha de estar en log10.
 * @return Resultado de la descomposicion. NO esta en log10.
 */
CImg<float>& Decoupling::reconstruct(const CImg<>& color, const CImg<>& details, const CImg<> largeScaleNF)const{
	// Creamos la reconstruccion empezando por el color
	CImg<>* recons=new CImg<>(color);

	// Al color le sumamos los detalles (hay que hacerlo capa a capa)
	recons->ref_channel(0)+=details;
	recons->ref_channel(1)+=details;
	recons->ref_channel(2)+=details;

	//pruebas
	/*recons->pow10();
	recons->display("color+details");
	recons->log10();*/
	//pruebas

	// Ahora sumamos la capa large scale sin flash
	recons->ref_channel(0)+=largeScaleNF;
	recons->ref_channel(1)+=largeScaleNF;
	recons->ref_channel(2)+=largeScaleNF;

	// Volvemos al dominio "normal"
	recons->pow10();
	//pruebas
	//recons->display("color+details+largescaleNF");
	//pruebas


	// Hago una normalizacion, el resultado es mucho mejor que con truncamiento
	recons->normalize(0,1);

	// Devolvemos el resultado
	return *recons;
}

/*
 * Ye olde stable algorithms.
 */

/**
 * Constructor.
 * @param imageFlash Image taken using flash.
 * Its values must be in the range [0,1].
 * @param imageNoFlash Imagen without flash, same conditions as the flash one.
 *
 * @ToDo: Check if they must be in Luv format.
 */
DecouplingStable::DecouplingStable(const CImg<>& imageFlash, const CImg<>& imageNoFlash)
	// Call base class constructor
	:Decoupling(imageFlash,imageNoFlash){
}

/**
 * Destructor.
 */
DecouplingStable::~DecouplingStable(){
	// Empty
}

/**
 * Calcula el filtro bilateral.
 * @param I Intesidad de la imagen
 * @return Imagen J, Large Scale.
 *
 * @ToDo: usar Fast Bilateral Filter.
 *
 * Atencion!!! Pequeño fallo al calcular la coordenada normalizada del pixel!
 * Divido entre (ejemplo) dimx, y nunca una coordenada llegara a dimx, por lo
 * que nunca tendre un pixel en 1,1. CUIDADO!!!
 */
CImg<float>& DecouplingStable::bilateralFilter(const CImg<float>& I)const{
	// La imagen J que devolvemos
	CImg<float>* J = new CImg<float>(I.dimx(),I.dimy());

	// Para todas las Xs
	for(int y=0;y<I.dimy();y++){
		cout << "\r" << y*100/I.dimy() << "%";
		// Para todas las Ys
		for(int x=0;x<I.dimx();x++){
			Pixel Is((float)x/I.dimx(),(float)y/I.dimy());
			// Calculamos Js=1/k(s) * SUM (...)
			// k(s) lo calculamos a la vez que el sumatorio
			float k=0;
			float sum=0;
			// Vuelvo a recorrer todos los pixeles de la imagen
			// Las coordenadas x2,y2 corresponden al pixel p
			for(int y2=0;y2<I.dimy();y2++){
				for(int x2=0;x2<I.dimy();x2++){
					// Pixel con el que comparar el actual
					Pixel Ip((float)x2/I.dimx(),(float)y2/I.dimy());
					// Producto de las gaussianas
					register float prod = Helper::gaussianF(Ip.euclidean(Is),_sigmaF);

					// Si el producto es 0, saltamos (mas rapido)
					if(prod==0.0f)
						continue;

					// Ahora multiplicamos por la gaussiana de similitud de intensidad
					prod*=Helper::gaussianG(I(x2,y2)-I(x,y),_sigmaG);

					// Lo mismo
					if(prod==0.0f)
						continue;

					// Le sumamos el producto a K
					k+=prod;
					// Y al sumatorio le sumamos el producto por la intensidad de p
					sum+=prod*I(x2,y2);
				}
			}
			// Ahora ya tenemos k(s) y el sumatorio:
			(*J)(x,y)=sum/k;
		}
	}
	cout << endl;

	// Devolvemos una nueva imagen del mismo tamaño que la I
	// pero con los valores calculados en bufferJ
	return *J;
}



/**
 * Calcula el filtro bilateral, excluyendo los píxeles con sombra.
 * @param I Intesidad de la imagen
 * @param shadow Mapa binario de la sombra.
 * @return Imagen J, Large Scale calculada sin la sombra.
 *
 * @ToDo: usar Fast Bilateral Filter.
 */
CImg<float>& DecouplingStable::bilateralFilter(const CImg<float>& I, const CImg<bool>& shadow)const{
	// La imagen J que devolvemos
	CImg<float>* J = new CImg<float>(I.dimx(),I.dimy());

	// Para todas las Ys
	for(int y=0;y<I.dimy();y++){
		cout << "\r" << y*100/I.dimy() << "%";
		// Para todas las Xs
		for(int x=0;x<I.dimx();x++){
			Pixel Is((float)x/I.dimx(),(float)y/I.dimy());
			// Calculamos Js=1/k(s) * SUM (...)
			// k(s) lo calculamos a la vez que el sumatorio
			float k=0;
			float sum=0;
			// Vuelvo a recorrer todos los pixeles de la imagen
			// Las coordenadas x2,y2 corresponden al pixel p
			for(int y2=0;y2<I.dimy();y2++){
				for(int x2=0;x2<I.dimx();x2++){
					// Si el pixel pertenece a la sombra
					if(shadow(x2,y2)/*==1*/){
						// No usamos el pixel, saltamos
						continue;
					}
					// Pixel con el que comparar el actual
					Pixel Ip((float)x2/I.dimx(),(float)y2/I.dimy());
					// Producto de las gaussianas
					//cerr << "euclidean p - s="<< Ip.euclidean(Is) << endl;
					register float prod = Helper::gaussianF(Ip.euclidean(Is),_sigmaF);

					// Si el producto es 0, saltamos (mas rapido)
					if(prod==0.0f)
						continue;

					// Ahora multiplicamos por la gaussiana de similitud de intensidad
					prod*=Helper::gaussianG(I(x2,y2)-I(x,y),_sigmaG);

					// Lo mismo de antes
					if(prod==0.0f)
						continue;

					// Le sumamos el producto a K
					k+=prod;
					// Y al sumatorio le sumamos el producto por la intensidad de p
					sum+=prod*I(x2,y2);
				}
			}
			// Ahora ya tenemos k(s) y el sumatorio:
			(*J)(x,y)=sum/k;
		}
	}
	cout << endl;

	// Devolvemos una nueva imagen del mismo tamaño que la I
	// pero con los valores calculados en bufferJ
	return *J;
}

/**
* Calcula la imagen large scale usando Cross
* Bilateral Filtering, para situaciones con muy poca iluminacion
* @param iNF Intesidad de la imagen sin flash.
* @param iF Intesidad de la imagen con flash.
* @note ¡Esto implica que no se podra hacer correcion de sombras!
* @return Imagen J, Large Scale.
*
* @ToDo: usar Fast Bilateral Filter.
*/
CImg<float>& DecouplingStable::crossBilateralFilter(const CImg<float>& iNF,const CImg<float>& iF)const{
	// La imagen J que devolvemos
	CImg<float>* J = new CImg<float>(iNF,false);

	// Para todas las ys
	for(int y=0;y<iNF.dimy();y++){
		cout << "\rY=" << y;
		// Para todas las xs
		for(int x=0;x<iNF.dimx();x++){
			Pixel Is((float)x/iNF.dimx(),(float)y/iNF.dimy());
			// Calculamos Js=1/k(s) * SUM (...)
			// k(s) lo calculamos a la vez que el sumatorio
			float k=0;
			float sum=0;
			// Vuelvo a recorrer todos los pixeles de la imagen
			// Las coordenadas x2,y2 corresponden al pixel p
			for(int y2=0;y2<iNF.dimx();y2++){
				for(int x2=0;x2<iNF.dimx();x2++){
					// Pixel con el que comparar el actual
					Pixel Ip((float)x2/iNF.dimx(),(float)y2/iNF.dimy());
					// Producto de las gaussianas
					register float prod = Helper::gaussianF(Ip.euclidean(Is),_sigmaF);

					// Si el producto es 0, saltamos (mas rapido)
					if(prod==0.0f)
						continue;

					prod*=Helper::gaussianG(iF(x2,y2)-iF(x,y),_sigmaG);

					// Lo mismo de antes
					if(prod==0.0f)
						continue;

					// Le sumamos el producto a K
					k+=prod;
					// Y al sumatorio le sumamos el producto por la intensidad de p
					sum+=prod*iNF(x2,y2);
				}
			}
			// Ahora ya tenemos k(s) y el sumatorio:
			(*J)(x,y)=sum/k;
		}
	}
	cout << endl;

	// Devolvemos una nueva imagen del mismo tamaño que la I
	// pero con los valores calculados en bufferJ
	return *J;

}


/*
 * Experimental implementation
 */
/**
 * Constructor.
 * @param imageFlash Image taken using flash.
 * Its values must be in the range [0,1].
 * @param imageNoFlash Imagen without flash, same conditions as the flash one.
 *
 * @ToDo: Check if they must be in Luv format.
 */
DecouplingExperimental::DecouplingExperimental(const CImg<>& imageFlash, const CImg<>& imageNoFlash)
	// Call base class constructor
	:Decoupling(imageFlash,imageNoFlash){
}

/**
 * Destructor.
 */
DecouplingExperimental::~DecouplingExperimental(){
	// Empty
}

/**
 * Calcula el filtro bilateral de manera alternativa.
 * Esta basado en el codigo de matlab escrito, aparentemente, por
 * Tomassi.
 * @param I Intesidad de la imagen
 * @return Imagen J, Large Scale.
 *
 *
 * Atencion!!! Pequeño fallo al calcular la coordenada normalizada del pixel!
 * Divido entre (ejemplo) dimx, y nunca una coordenada llegara a dimx, por lo
 * que nunca tendre un pixel en 1,1. CUIDADO!!!
 */
CImg<float>& DecouplingExperimental::bilateralFilter(const CImg<float>& I)const{
	// La imagen J que devolvemos
	CImg<float>* J = new CImg<float>(I,false);

	// Esta c_r es para la varianza de intensidades
	float c_r=1.0/(2.0f*(_sigmaG*_sigmaG));
	// Y Esta para la espacial
	float c_d=1.0/(2.0f*(_sigmaF*_sigmaF));

	// Esto es la mitad del tamaño de la ventana
	int hwin=(int)Helper::max(1,std::ceil(2.1*_sigmaF));
	//cerr << "Decoupling::bilateralFilterAlt hwin=" << hwin << endl;

	// Construimos squares y gaussian a la vez
	CImg<> gaussian(2*hwin+1);
	for(unsigned int p=0;p<gaussian.size();p++){
		gaussian[p]=(p-hwin)*(p-hwin);
	}
	// Ahora hacemos la parte gaussiana
	for(unsigned int p=0;p<gaussian.size();p++){
		gaussian[p]=exp(-c_d*gaussian[p]);
	}
	//gaussian.display("gaussian");
	//gaussian.get_transpose().display("transpuesta");
	// Aqui parece que es al reves que en matlab (los vectores son horizontales)
	/// @ToDo comprobar como son en matlab
	CImg<> gaussian_d=gaussian.get_transpose()*gaussian;
	//gaussian_d.display("gaussian d");

	// Creamos la imagen en blanco K, que se corresponde con la d
	// del codigo original en matlab
	CImg<> K(I,false);

	// Para cada fila de la ventana
	for(int r=-hwin;r<=hwin;r++){
		cout << "\r" << (r+hwin)*100/(hwin*2) <<"%";
		// Para cada columna de la ventana
		for(int c=-hwin;c<=hwin;c++){
			// En matlab se le suma 1 a las coordenadas xq el rango es de 1 a N
			float g=gaussian_d(c+hwin,r+hwin);

			// Imagen con la suma
			CImg<> s(I,false);
			// Imagen con algo...
			CImg<> is(I,false);

			// Aqui tampoco sumamos 1
			int rs=(int)Helper::max(0,r);
			// Creo que tendria que restar -1
			int re=I.dimy()+(int)Helper::min(0,r)-1;
			int cs=(int)Helper::max(0,c);
			int ce=I.dimx()+(int)Helper::min(0,c)-1;

			for(int r2=rs;r2<=re;r2++){
				for(int c2=cs;c2<=ce;c2++){
					is(c2,r2)=I(c2-c,r2-r);
				}
			}

			for(int r2=rs;r2<=re;r2++){
				for(int c2=cs;c2<=ce;c2++){
					// Calculamos la diferencia
					float diferencia=I(c2,r2)-is(c2,r2);
					// La gaussiana de la intensidad es e^(-c_r*diferencia cuadrada)
					s(c2,r2)=exp(-c_r*(diferencia*diferencia))*g;
				}
			}

			// Ahora incrementamos la K
			K+=s;
			// Y añadimos los nuevos pixeles a la imagen resultante
			(*J)+=s.mul(is);
		}
	}

	cout << endl;

	// Dividimos la imagen por el factor corrector K
	(*J).div(K);

	//((*J)+K).display("J+K");
	// Devolvemos una nueva imagen del mismo tamaño que la I
	// pero con los valores calculados en bufferJ
	return *J;
}

/**
 * Calcula el filtro bilateral, excluyendo los píxeles con sombra, de manera alternativa.
 * Esta basado en el codigo de matlab escrito, aparentemente, por
 * Tomassi.
 * @param I Intesidad de la imagen
 * @param shadow Mapa binario de la sombra.
 * @return Imagen J, Large Scale.
 *
 * Atencion!!! Pequeño fallo al calcular la coordenada normalizada del pixel!
 * Divido entre (ejemplo) dimx, y nunca una coordenada llegara a dimx, por lo
 * que nunca tendre un pixel en 1,1. CUIDADO!!!
 */
CImg<float>& DecouplingExperimental::bilateralFilter(const CImg<float>& I, const CImg<bool>& shadow)const{
	// La imagen J que devolvemos
	CImg<float>* J = new CImg<float>(I,false);

	// Esta c_r es para la varianza de intensidades
	float c_r=1.0/(2.0f*(_sigmaG*_sigmaG));
	// Y Esta para la espacial
	float c_d=1.0/(2.0f*(_sigmaF*_sigmaF));

	// Esto es la mitad del tamaño de la ventana
	int hwin=(int)Helper::max(1,std::ceil(2.1*_sigmaF));
	//cerr << "Decoupling::bilateralFilterAlt hwin=" << hwin << endl;

	// Construimos squares y gaussian a la vez
	CImg<> gaussian(2*hwin+1);
	for(unsigned int p=0;p<gaussian.size();p++){
		gaussian[p]=(p-hwin)*(p-hwin);
	}
	// Ahora hacemos la parte gaussiana
	for(unsigned int p=0;p<gaussian.size();p++){
		gaussian[p]=exp(-c_d*gaussian[p]);
	}

	CImg<> gaussian_d=gaussian.get_transpose()*gaussian;

	// Creamos la imagen en blanco K, que se corresponde con la d
	// del codigo original en matlab
	CImg<> K(I,false);

	// Para cada fila de la ventana
	for(int r=-hwin;r<=hwin;r++){
		cout << "\r" << (r+hwin)*100/(hwin*2) <<"%";
		// Para cada columna de la ventana
		for(int c=-hwin;c<=hwin;c++){
			// En matlab se le suma 1 a las coordenadas xq el rango es de 1 a N
			float g=gaussian_d(c+hwin,r+hwin);

			// Imagen con la suma
			CImg<> s(I,false);
			// Imagen con algo...
			CImg<> is(I,false);

			// Aqui tampoco sumamos 1
			int rs=(int)Helper::max(0,r);
			// Creo que tendria que restar -1
			int re=I.dimy()+(int)Helper::min(0,r)-1;
			int cs=(int)Helper::max(0,c);
			int ce=I.dimx()+(int)Helper::min(0,c)-1;

			for(int r2=rs;r2<=re;r2++){
				for(int c2=cs;c2<=ce;c2++){
					is(c2,r2)=I(c2-c,r2-r);
				}
			}

			for(int r2=rs;r2<=re;r2++){
				for(int c2=cs;c2<=ce;c2++){
					//if(shadow(c2-c,r2-r))
					/*if(shadow(c2,r2))
					continue;*/
					// Calculamos la diferencia
					float diferencia=I(c2,r2)-is(c2,r2);
					// La gaussiana de la intensidad es e^(-c_r*diferencia cuadrada)
					s(c2,r2)=shadow(c2-c,r2-r)? Helper::fakezero : exp(-c_r*(diferencia*diferencia))*g;
				}
			}

			// Ahora incrementamos la K
			K+=s;
			// Y añadimos los nuevos pixeles a la imagen resultante
			(*J)+=s.mul(is);
		}
	}

	cout << endl;

	// Dividimos la imagen por el factor corrector K
	(*J).div(K);

	//((*J)+K).display("J+K");
	// Devolvemos una nueva imagen del mismo tamaño que la I
	// pero con los valores calculados en bufferJ
	return *J;
}

/**
 * Calcula la imagen large scale usando Cross
 * Bilateral Filtering, para situaciones con muy poca iluminacion
 * @param iNF Intesidad de la imagen sin flash.
 * @param iF Intesidad de la imagen con flash.
 * @note ¡Esto implica que no se podra hacer correcion de sombras!
 * @return Imagen J, Large Scale.
 *
 * @ToDo: usar Fast Bilateral Filter.
*/
CImg<float>& DecouplingExperimental::crossBilateralFilter(const CImg<float>& iNF,const CImg<float>& iF)const{
	// La imagen J que devolvemos
	CImg<float>* J = new CImg<float>(iNF,false);

	// Esta c_r es para la varianza de intensidades
	float c_r=1.0/(2.0f*(_sigmaG*_sigmaG));
	// Y Esta para la espacial
	float c_d=1.0/(2.0f*(_sigmaF*_sigmaF));

	// Esto es la mitad del tamaño de la ventana
	int hwin=(int)Helper::max(1,std::ceil(2.1*_sigmaF));
	cerr << "DecouplingExperimental::bilateralFilter hwin=" << hwin << endl;

	// Construimos squares y gaussian a la vez
	CImg<> gaussian(2*hwin+1);
	for(unsigned int p=0;p<gaussian.size();p++){
		gaussian[p]=(p-hwin)*(p-hwin);
	}
	// Ahora hacemos la parte gaussiana
	for(unsigned int p=0;p<gaussian.size();p++){
		gaussian[p]=exp(-c_d*gaussian[p]);
	}
	//gaussian.display("gaussian");
	//gaussian.get_transpose().display("transpuesta");
	CImg<> gaussian_d=gaussian.get_transpose()*gaussian;
	//gaussian_d.display("gaussian d");

	// Creamos la imagen en blanco K, que se corresponde con la d
	// del codigo original en matlab
	CImg<> K(iNF,false);

	// Para cada fila de la ventana
	for(int r=-hwin;r<=hwin;r++){
		cout << "\rR=" << r <<" ";
		// Para cada columna de la ventana
		for(int c=-hwin;c<=hwin;c++){
			// En matlab se le suma 1 a las coordenadas xq el rango es de 1 a N
			float g=gaussian_d(c+hwin,r+hwin);

			// Imagen con la suma
			CImg<> s(iNF,false);
			// Imagen con algo...
			CImg<> is(iNF,false);

			// Aqui tampoco sumamos 1
			int rs=(int)Helper::max(0,r);
			// Creo que tendria que restar -1
			int re=iNF.dimy()+(int)Helper::min(0,r)-1;
			int cs=(int)Helper::max(0,c);
			int ce=iNF.dimx()+(int)Helper::min(0,c)-1;

			for(int r2=rs;r2<=re;r2++){
				for(int c2=cs;c2<=ce;c2++){
					is(c2,r2)=iF(c2-c,r2-r);
				}
			}

			for(int r2=rs;r2<=re;r2++){
				for(int c2=cs;c2<=ce;c2++){
					// Calculamos la diferencia
					float diferencia=iF(c2,r2)-is(c2,r2);
					// La gaussiana de la intensidad es e^(-c_r*diferencia cuadrada)
					s(c2,r2)=exp(-c_r*(diferencia*diferencia))*g;
				}
			}

			// Ahora incrementamos la K
			K+=s;
			// Y añadimos los nuevos pixeles a la imagen resultante
			(*J)+=s.mul(/*is*/iNF);
		}
	}

	cout << endl;

	// Dividimos la imagen por el factor corrector K
	(*J).div(K);

	//((*J)+K).display("J+K");
	// Devolvemos una nueva imagen del mismo tamaño que la I
	// pero con los valores calculados en bufferJ
	return *J;
}

/**
 * Crea un kernel gaussiano de la dimension especificada.
 * @param size Tamaño del kernel (3,5,7). Por defecto vale 5.
 * @param sigma Varianza. Por defecto es 1.
 * @return Kernel gaussiano.
 */
CImg<>& DecouplingExperimental::gaussianKernel(const int size, const float sigma){
	//cerr << "Decoupling::gaussianKernel("<<size<<")" << endl;
	// Por ahora calcula solo de 5
	CImg<>* kernel=new CImg<>(size,size);
	// El origen de la matriz esta en el centro (2,2)
	Pixel centro((float)2/(size-1),(float)2/(size-1));
	// Ahora comparo todos las posiciones
	for(int y=0;y<size;y++){
		cerr << "y="<<y<<endl;
		for(int x=0;x<size;x++){
			//cerr << "x="<<x<<endl;
			Pixel actual((float)x/(size-1),(float)y/(size-1));
			//cerr << "Punto actual: " << actual.getX() <<","<<actual.getY() << endl;
			//cerr << "euclidean: " << actual.euclidean(centro) << endl;
			(*kernel)(x,y)=Helper::gaussianF(actual.euclidean(centro),sigma);
		}
	}
	//cerr << "Suma=" << kernel->sum() << endl;
	(*kernel)/=kernel->sum();
	return *kernel;
}

/**
 * Calcula la interpolacion lineal de los dos valores mas cercanos a
 * ij de la imagen original I.
 * @param I imagen original.
 * @param ij valor i^j.
 * @return Valor de la interpolacion....
 */
CImg<>& DecouplingExperimental::interpolationWeight(const CImg<>& I, const float ij)const{
	CImg<>* res=new CImg<>(I,false);

	for(unsigned int p=0;p<I.size();p++){
		if((I[p]<=ij-1.0f)||(I[p]>=ij+1.0f))
			(*res)[p]=0.0f;
		else if((I[p]>ij-1.0f)&&(I[p]<=ij))
			(*res)[p]=I[p]+1.0f;
		else if((ij>0.0f)&&(ij<1.0f))
			(*res)[p]=1.0f-I[p];
	}

	return *res;
}

/**
 * Calcula el filtro bilateral "piecewise"
 * @param I Intesidad de la imagen
 * @return Imagen J, Large Scale.
 * @ToDo comprobar si se tiene que hacer el NB_SEGMENTS esimo segmento.
 * @ToDo poder cambiar el tamaño del kernel gaussiano
 * @ToDo Implementar interpolation weight
 * @ToDo Optimizar con saltar la WI.
*/
CImg<>& DecouplingExperimental::piecewiseBilateralFilter(const CImg<float>& I)const{
	cerr << "DecouplingExperimental::piecewiseBilateralFilter()" << endl;
	// La imagen J que devolvemos
	CImg<>* J = new CImg<>(I,false);


	// Image statistics (for min and max)
	const CImgStats stats(I);
	// Maximum and min pixel of the image
	float maxI=stats.min,minI=stats.min;

	// Kernel gaussiano espacial
	CImg<> f = gaussianKernel(5,_sigmaF);
	// Y su fft
	CImgl<> fFFT = Helper::getFFT(f);

	// Se calcula el numero de segmentos
	int NB_SEGMENTS = /*static_cast<int>( (maxI-minI)/_sigmaG)*/17;
	cerr << "NB_SEGMENTS = " << NB_SEGMENTS << endl;

	// Para todas las Xs
	/*for(int y=0;y<I.dimy();y++){
		cerr << "\rY=" << y;
		// Para todas las Ys
		for(int x=0;x<I.dimx();x++){
		// La salida es 0 al principio*/
	//(*J)(x,y)=0;
	// Ahora para cada uno de los segmentos
	for(int j=0;j<=NB_SEGMENTS;j++){
			// Puedo  comprobar aqui la interpolacion, si es 0 salto
		float i=minI+j*(float)(maxI-minI)/NB_SEGMENTS;
		/*
		* Ahora la G^j sera una imagen donde en cada pixel
		* esta la evaluacion de la influencia de la intensidad.
		* Para cada pixel he calculado la gaussiana de diferencia
		* de la intensidad entre I e i^j (ese float i).
		*/
		CImg<> G(I,false);
		for(unsigned int pixel=0;pixel<G.size();pixel++){
			G[pixel]=Helper::gaussianG(I[pixel]-i,_sigmaG);
		}
		//G.display("G");
		/*
		* Ahora hago la convolucion entre G^j y f, es decir,
		* entre el kernel gaussiano de espacio, y obtengo K^j.
		*/
		CImg<> K = G.get_convolve(f);
		//K.display("K");
		/*CImgl<> kFFT = Helper::getFFT(G);

		kFFT[0].mul(fFFT[0]);
		kFFT[1].mul(fFFT[1]);
		CImg<> K = Helper::getiFFT(kFFT,G.dimx(),G.dimy());*/


		/*
		* Calculo la H, que es la multiplicacion pixel a pixel
		* de G por la intensidad
		*/
		CImg<> H = G.get_mul(I);
		//H.display("H");


		// Ahora H estrella, que es la convolucion entre H y f
		CImg<> Hstar = H.convolve(f);
		/*CImgl<> HstarFFT = Helper::getFFT(H);
		HstarFFT[0].mul(fFFT[0]);
			//HstarFFT[1].mul(fFFT[1]);
		CImg<> Hstar = Helper::getiFFT(HstarFFT,H.dimx(),H.dimy());*/

		// Ahora J', que es normalizar la H estrella con K
		CImg<> Jp = Hstar.div(K);
		//Jp.display("J'");
		// Aqui debajo habria que multiplciar por interpolation weight
		//(*J)+=Jp/*.mul(interpolationWeight(I,i))*/*i;
		for(unsigned int pixel=0;pixel<G.size();pixel++){
			float wi=((maxI-minI)/NB_SEGMENTS-std::abs(I[pixel]-i))
				/((maxI-minI)/NB_SEGMENTS);
			(*J)[pixel]+=Jp[pixel]*wi;
		}

	}
	/*}
}*/
	cerr << endl;

// Devolvemos una nueva imagen del mismo tamaño que la I
// pero con los valores calculados en bufferJ
	return *J;
}
