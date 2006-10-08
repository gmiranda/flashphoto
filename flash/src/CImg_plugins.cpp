
/**
 * Replace each image pixel by 10 power by its value.
 * @see log10(), get_log10(), get_pow(), pow()
 */
CImg& pow10() {
    cimg_map(*this,ptr,T) (*ptr)=(T)std::pow((double)10.0,(double)(*ptr));
    return *this;
}
/**
 * Computes the length of the image's diagonal.
 * @return longitud de la diagonal.
 */
float diagonalLength()const{
	return std::sqrt(width*width+height*height);
}
