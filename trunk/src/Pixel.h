#ifndef _Pixel_h_
#define _Pixel_h_

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
