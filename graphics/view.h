#pragma once
#include "image.h"

struct Viewport {
	double width; 
	double height;

	Viewport() : width(0.0), height(0.0) {};
	// Create a viewport with the specified dimensions.
	Viewport(double width, double height) : width(width), height(height) {
		this->width = width;
		this->height = height;
	}
};

class Camera {
private: 
	Image* image;
	Viewport viewport;
	double focalLength;

public: 
	// Create a camera that produces an image with the given dimensions. 
	Camera(size_t imageWidth, size_t imageHeight, const Viewport& viewport);

	// Create an image of the scene and return it. 
	Image& render();
};