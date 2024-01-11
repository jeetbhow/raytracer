#pragma once
#include "transform.h"
#include <vector>

class Sphere {
public:
	// Tells you how to get from object to world space. 
	Mat4 transform;

	// Returns the inverse of the transform matrix. 
	Mat4 invert() const;

	// Create a unit sphere. 
	Sphere();

	// Create a sphere with the given center and radius. 
	Sphere(Pnt3 center, double radius);

	// Return the radius of this sphere. 
	double radius() const;

	// Return the center of this sphere. 
	Pnt3 center() const;

	// Translate this sphere. 
	Sphere& translate(double dx, double dy, double dz);

	// Scale this sphere.
	Sphere& scale(double kx, double ky, double kz);

	// Return the time it takes for a ray to hit this sphere. 
	std::vector<double> hit(Ray& r) const;

	// Return the surface normal vector at point p. The surface normal will not be normalized
	// so you will have to normalize it afterwards when doing shading calculations. 
	Vec3 normal(const Pnt3& p) const;
};



