#include "image.h"
#include <iostream>
#include <format>

Image::Image(size_t width, size_t height) {
	this->width = width;
	this->height = height;
	imgbuf = std::vector<unsigned char>(4 * width * height);
}

Color Image::getPixel(size_t row, size_t col) const {
	const size_t baseIndex = row * width + col;
	double r = imgbuf[baseIndex] / 255.0;
	double g = imgbuf[baseIndex + 1] / 255.0;
	double b = imgbuf[baseIndex + 2] / 255.0;
	double a = imgbuf[baseIndex + 3] / 255.0;
	return Color{ r, g, b, a };
}

void Image::setPixel(size_t row, size_t col, const Color& color) {
	const size_t baseIndex = 4 * (row * width + col);
	imgbuf[baseIndex] = static_cast<unsigned char>(255.999 * color.r);
	imgbuf[baseIndex + 1] = static_cast<unsigned char>(255.999 * color.g);
	imgbuf[baseIndex + 2] = static_cast<unsigned char>(255.999 * color.b);
	imgbuf[baseIndex + 3] = static_cast<unsigned char>(255.999 * color.a);
}

size_t Image::getWidth() const {
	return width;
}

size_t Image::getHeight() const {
	return height;
}

double Image::aspectRatio() const {
	return static_cast<double>(width) / static_cast<double>(height);
}

bool Image::save(std::string path) {
	unsigned error = lodepng::encode(path, imgbuf, static_cast<unsigned>(width), static_cast<unsigned>(height));
	if (error) {
		std::cout << "Error " << error << ": " << lodepng_error_text(error) << std::endl;
		return false;
	}
	return true;
}


std::ostream& operator<<(std::ostream& os, const Color& p) {
	os << std::format("<{}, {}, {}>", p.r, p.g, p.b);
	return os;
}
