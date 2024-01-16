#pragma once
#include "lodepng.h"
#include "light.h"
#include <vector>

class Image {
private: 
	std::vector<unsigned char> imgbuf;
	size_t width, height;

public: 
	// Create an image with the given dimensions. 
	Image(size_t width, size_t height);
	
	// Get the pixel located at the specified row and col.
	Color getPixel(size_t row, size_t col) const;

	// Set the pixel at the specified row and col.
	void setPixel(size_t row, size_t col, const Color& color);

	// Return the width of the image.
	size_t getWidth() const;

	// Return the height of the image.
	size_t getHeight() const;

	// Get the aspect ratio of the image. 
	double aspectRatio() const;

	// Save the image at a desired path
	bool save(std::string path);
};