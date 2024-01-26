#pragma once
#include "geometry.h"
#include <algorithm>

// Encodes an RGBA value. Colors can be added together and multiplied for shading. 
struct Color {
	double r;
	double g;
	double b;
	double a;

	// Create a pixel with default alpha (1.0). 
	Color(double r, double g, double b) : r(r), g(g), b(b), a(1.0f) {};

	// Create a pixel with the given rgba values. 
	Color(double r, double g, double b, double a) : r(r), g(g), b(b), a(a) {};

	// Multiply a color by a scalar. 
	Color operator *(const double scalar) const;

	// Multiply a color by another color. The product of two colors is the product of the individual channels. 
	Color operator *(const Color& c) const;

	// Add two colors together. The sum of two colors is the sum of the individual channels.
	Color operator +(const Color& c) const;

	// Subtract two colors. The different between two colors is the difference between their individual channels.
	Color operator -(const Color& c) const;

	// Increment this color by another one. Adds the other color's rgb values to this color. 
	void operator +=(const Color& c);

	// Clamp the rgb channels so that their values are between 0 and 1. 
	void clamp();

	// Send a string representation of the pixel to an output stream. 
	friend std::ostream& operator <<(std::ostream& os, const Color& p);

	// Return a string representation of the color.
	std::string toString() const;
};

// A point light.
struct Light {
	Pnt3 location;
	Color color;
};