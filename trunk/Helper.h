#ifndef _Helper_h_
#define _Helper_h_

/// Biblioteca de calculos matematicos
#include <cmath>

/// Incluimos la biblioteca de CImg
#include "CImg.h"
using namespace cimg_library;

/// Cosas de la biblioteca estandar
#include <iostream>
using std::cerr;
using std::endl;

/// Definimos Pi
#define PI 3.14159265f

/**
 * Clase de ayuda, con metodos estaticos.
 * 
 * Los metodos de esta clase son empleados por el resto
 * de las clases de la practica, pero, pese a que colaboran
 * en los metodos de las otras clases, son solo herramientas
 * que usan esos metodos.
 * 
 * Por ejemplo: la clase Shadow, utiliza el metodo
 * boxFilter (que crea un filtro similar al box) en el
 * metodo penumbra, pero la finalidad de penumbra no es
 * crear el filtro.
 */
class Helper{
	/// No hace falta instanciar :)
	Helper();
	
public:	
	/**
	 * Devuelve el minimo de los dos numeros.
	 */
	static float min(const float a,const float b){
		return a<=b?a:b;
	}
	
	/**
	 * Devuelve el maximo de los dos numeros.
	 */
	static float max(const float a,const float b){
		return a>=b?a:b;
	}
	
	/**
	 * Calcula la longitud de la diagonal de la imagen.
	 * @param image imagen fuente.
	 * @return longitud de la diagonal.
	 */
	static float diagonalLength(const CImg<>& image){
		return sqrt(image.dimx()*image.dimx()+image.dimy()*image.dimy());
	}

	/**
	 * Funcion de gaussiana de proximidad entre dos pixeles.
	 * La utilizamos para el bilateral filter.
	 * @param dist Distancia euclidiana entre los dos pixeles
	 * @param sigmaF Varianza.
	 * @return Resultado de aplicar la funcion de Gauss al
	 * numero de entrada con la sigma proporcionada.
	 * @see Bilateral Filtering for Gray and Color Images
	 * Proceedings of the IEEE, 1998 [pag. 3]
	 *
	 * @ToDo optimizar (ej: precalculando el cuadrado de sigmaF.
	 */
	static float gaussianF(const double dist, const float sigmaF){
		// La gaussiana es e^(-1/2 *( ||u-x|| / sigma )^2 )
		return (float)(1/(sigmaF*std::sqrt(2*PI)))*(exp(-0.5f*((dist/sigmaF)*(dist/sigmaF))));

	}

	/**
	 * Funcion gaussiana de similitud.
	 * @param diff Diferencia de intensidad de los pixeles.
	 * @param sigmaG Varianza.
	 * @return Resultado de la funcion gaussiana de similitud
	 * aplicada a la diferencia de intesidad y sigma dados.
	 * @see Bilateral Filtering for Gray and Color Images
	 * Proceedings of the IEEE, 1998 [pag. 3]
	 * @ToDo optimizar.
	 */
	static float gaussianG(const float diff, const float sigmaG){
		return (float)(1/(sigmaG*std::sqrt(2*PI)))*(exp(-0.5f*((fabs(diff)/sigmaG)*(fabs(diff)/sigmaG))));
		//return (float)exp(-0.5f*((fabs(diff)/sigmaG)*(fabs(diff)/sigmaG)));
	}
	
	/**
	 * Aplica un truncamiento a los valores de los pixeles
	 * de la imagen para que esten en el rango [0,1].
	 * @param image Imagen con valores reales, que se modificaran
	 * para estar en [0,1].
	 * @param max Valor maximo (por defecto 1.0);
	 * @param min Valor minimo (por defecto 0.0);
	 */
	static void truncate(CImg<float>& image, float max=1.0f, float min=0.0f){
		// Obtenemos el array de datos de la imagen
		float* data = image.ptr();
		
		// Recorremos el array
		for(unsigned int i=0;i<image.size();i++){
			// Si el valor es < 0 (o el minimo)
			if(data[i]<min){
				// Cambiamos el valor
				data[i]=min;
				// Pasamos al siguiente elemento
				continue;
			}
			// Y si es >1 (maximo)
			if(data[i]>max){
				// Cambiamos el valor
				data[i]=max;
			}
		}
	}
	
	/**
	 * Aplica un truncamiento a los valores de los pixeles
	 * de la imagen para que esten en el rango [0,1], y 
	 * devuelve el resultado.
	 * @param image Imagen con valores reales.
	 * @param max Valor maximo a truncar.
	 * @param min Valor minimo a truncar.
	 * @return Nueva imagen con valores en [0,1].
	 */
	static CImg<>& getTruncate(const CImg<float>& image, float max=1.0f, float min=0.0f){
		// Creamos una nueva imagen de las dimensiones de image,
		// pero sin copiar sus pixeles
		CImg<float>* imageT=new CImg<float>(image,false);
		// Obtenemos el array de datos de la imagen original
		float* data = image.ptr();
		// Ahora el de la nueva imagen
		float* dataT = imageT->ptr();
		// Recorremos el array
		for(unsigned int i=0;i<image.size();i++){
			// Si el valor es < 0 (o el minimo)
			if(data[i]<min){
				// Cambiamos el valor
				dataT[i]=min;
				// Pasamos al siguiente elemento
				continue;
			}
			// Y si es >1 (maximo)
			if(data[i]>max){
				// Cambiamos el valor
				dataT[i]=max;
				// Siguiente
				continue;
			}
			// Sino, copiamos el valor
			dataT[i]=data[i];
		}
		return *imageT;
	}
	
	/**
	 * Devuelve el gradiente (magnitud) de una imagen.
	 * @param image Imagen origen.
	 * @return Nueva imagen gradiente (magnitud).
	 */
	static CImg<>& get_magnitude_gradient(const CImg<>& image){
		// Calculamos el gradiente horizontal y vertical
		CImgl<> gradiente = image.get_gradientXY();
		gradiente[0].pow(2); 
		gradiente[1].pow(2); 
		// Y ahora un puntero al gradiente en magnitud
		CImg<>* mag = new CImg<>((gradiente[0]+gradiente[1]).sqrt());
		// Devolvemos referencia
		return *mag;
	}

	/**
	 * Crea un box filter, de 3x3.
	 * @return Box filter de 3x3.
	 */
	static CImg<> boxFilter(){
		return CImg<>(3,3,1,1,1.0f/9.0f);
	}
	
	/**
	 * Devuelve un filtro similar a box filter,
	 * pero de tamaño segun la diagonal de la imagen.
	 * @param image Imagen de la que se obtendra la diagonal.
	 * @param percentage Porcentaje de la diagonal que se usara
	 * para el tamaño del filtro.
	 * @return Filtro parecido al box filter.
	 * @note Si ese 1% de la diagonal es un numero par, le sumo 1
	 * para que el filtro tenga la forma correcta. Es decir, si
	 * 1% de la diagonal es 4, las dimensiones del filtro tipo box
	 * serian de 5x5.
	 * 
	 * La otra opcion,quizas menos deseable, seria restarle uno.
	 */
	static CImg<> boxFilter(const CImg<> image, float percentage){
		// Obtenemos el tamaño de la diagonal
		float diagonal = diagonalLength(image);

		// Multiplicamos el tamaño de la diagonal por el porcentaje
		// y redondeamos
		unsigned int num = (unsigned int)round(diagonal*percentage);
		
		// Miramos si es par
		if(num%2==0){
			// Si es par, le sumamos uno
			//cerr << "Es par (" << num << "), sumo 1" << endl;
			num++;
		}
		// Si es impar
		else{
			// No hace falta sumarle
			//cerr << "Es impar (" << num << ")" << endl;
		}
		
		/*
		 * Creamos el filtro y lo devolvemos
		 * El filtro ha de hacer un suavizado uniforme:
		 * todas las casillas han de tener el mismo valor,
		 * y en total sumar 1. Para ello hay que dividir entre
		 * el numero de casillas (num*num)
		 */
		return CImg<>(num,num,1,1,1.0f/(num*num));
	}
	
	/**
	 * Este metodo calcula el numero potencia de dos inmediatamente
	 * superior al dado.
	 * 
	 * Es util para poder redimensionar imagenes antes de hacer su FFT,
	 * ya que el requisito es que las dimensiones sean potencia de 2.
	 * @param num Numero.
	 * @return La potencia de 2 mas proxima al numero dado, por arriba.
	 */
	static int upper2(const int num){
		// Primero calculamos el exponente
		int exp = (int)std::ceil(std::log((double)num)/std::log(2.0f));
		// Y devolvemos 2 elevado a ese numero
		return (int)std::pow(2.0f,exp);
	}
	
	/**
	 * Metodo que calcula la FFT de cualquier imagen.
	 * Rellena con ceros la imagen para que tenga las dimensiones
	 * necesarias para la FFT.
	 * @param image Imagen origen.
	 * @return Lista de imagenes resultado de la FFT.
	 * @see upper2
	 * @note Solo ha sido probado con imagenes de un solo color.
	 */
	static CImgl<> getFFT(const CImg<>& image){
		// Creamos la imagen de dimensiones potencia de 2
		CImg<> imageP=image.get_resize(upper2(image.dimx()),upper2(image.dimy()),image.dimz(),image.dimv(),0);
		imageP.display("Imagen redimensionada");
		return imageP.get_FFT();
	}
	
	/**
	 * Este metodo devuelve la FFT inversa, a partir de una lista de imagenes
	 * en dominio frecuenial.
	 * @param imageFFT Imagen en dominio frecuencial, realmente es una lista
	 * con dos imagenes.
	 * @param dimx Dimension de la imagen en el eje de las x.
	 * @param dimy Dimension de la imagen en el eje de las y.
	 * @ToDo investigar que es cada una de esas dos imagenes.
	 */
	static CImg<>& getiFFT(const CImgl<>& imageFFT, const int dimx, const int dimy){
		// Se calcula la inversa de la FFT
		CImgl<> intenFFTi = imageFFT.get_FFT(true);
		
		/*
		 * Se hace la suma de las dos imagenes
		 * y se recorta al tamaño original
		 */
		CImg<>* inversa = new CImg<>((intenFFTi[0]+intenFFTi[1]).crop(0,0,dimx,dimy));
		// Se devuelve el resultado
		return *inversa;
	}
};

#endif
