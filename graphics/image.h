#pragma once
#include "lodepng.h"
#include <vector>

struct Pixel {
	float r;
	float g;
	float b;
	float a;

	// Create a pixel with default alpha (1.0). 
	Pixel(float r, float g, float b) : r(r), g(g), b(b), a(1.0f) {};

	// Create a pixel with the given rgba values. 
	Pixel(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {};

	// Send a string representation of the pixel to an output stream. 
	friend std::ostream& operator <<(std::ostream& os, const Pixel& p);
};

class Image {
private: 
	std::vector<unsigned char> imgbuf;
	size_t width, height;

public: 
	// Create an image with the given dimensions. 
	Image(size_t width, size_t height);
	
	// Get the pixel located at the specified row and col.
	Pixel getPixel(size_t row, size_t col) const;

	// Set the pixel at the specified row and col.
	void setPixel(size_t row, size_t col, const Pixel& color);

	// Return the width of the image.
	size_t getWidth() const;

	// Return the height of the image.
	size_t getHeight() const;

	// Get the aspect ratio of the image. 
	double aspectRatio() const;

	// Save the image at a desired path
	bool save(std::string path);
};



