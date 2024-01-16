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

	// These operations allow you to compose colors together and also manipulate the intensity.
	Color operator *(double scalar) const { return Color(r * scalar, g * scalar, b * scalar); }
	Color operator *(const Color& other) const { return Color{ r * other.r, g * other.g, b * other.b }; }

	Color operator +(const Color& other) const { 
		return Color{
			std::min(1.0, r + other.r), 
			std::min(1.0, g + other.g), 
			std::min(1.0, b + other.b) 
		}; 
	}
	
	Color operator -(const Color& other) const { 
		return Color{ 
			std::max(0.0, r - other.r), 
			std::max(0.0, g - other.g), 
			std::max(0.0, b - other.b)
		}; 
	}

	void operator +=(const Color& other) { r += other.r;  g += other.g; b += other.b; }

	// Send a string representation of the pixel to an output stream. 
	friend std::ostream& operator <<(std::ostream& os, const Color& p);
};

// A point light.
class Light {
private:
	Pnt3 location;
	Color color;

public:
	Light(const Color& color, Pnt3 location) : color(color), location(location) {}
	Pnt3 getLocation() const { return location; }
	Color getColor() const { return color; }
};