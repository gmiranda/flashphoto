#ifndef _Pixel_h_
#define _Pixel_h_

// Libreria de calculos matematicos parala raiz cuadrada
#include <cmath>

/**
 * Clase para almacenar con coordenadas de pixeles.
 *
 * Como es una clase pequeña con metodos poco complejos,
 * esta solo en este .h, y los metodos son inline.
 */
class Pixel{
	/// Coordenada en el eje de las X
	float _x;
	/// Coordenada en el eje de las Y
	float _y;
public:
	/**
	 * Construye un pixel a partir de sus valores.
	 * @param x Coordenada en el eje de las X.
	 * @param y Coordenada en el eje de las Y
	 */
	Pixel(const float x, const float y){
		_x=x;
		_y=y;
	}

	/**
	 * Destructor.
	 * Por ahora no hace nada.
	 */
	virtual ~Pixel(){
	}

	/**
	 * Obtiene la coordenada en las Xs.
	 * @return Coordenada en el eje X.
	 */
	float getX()const{
		return _x;
	}

	/**
	 * Obtiene la coordenada en las Y.
	 * @return Coordenada en el eje Y.
	 */
	float getY()const{
		return _y;
	}

	/**
	 * Cambia la coordenada X del pixel.
	 * @param x Nueva coordenada en las Xs.
	 */
	void setX(const float x){
		_x=x;
	}
	
	/**
	 * Cambia la coordenada Y del pixel.
	 * @param y Nueva coordenada en el eje Y.
	 */
	void setY(const float y){
		_y=y;
	}

	/**
	 * Funcion que calcula la distancia euclidiana
	 * entre este pixel y el proporcionado.
	 * @param p2 Pixel con el que comparar.
	 * @return Distancia euclidiana.
	 */
	double euclidean(const Pixel& p2)const{
		double param = (_x-p2._x)*(_x-p2._x)+(_y-p2._y)*(_y-p2._y);
		return std::sqrt(param);
	}
};


#endif
