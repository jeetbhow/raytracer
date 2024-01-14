#pragma once
#include <vector>
#include "transform.h"

class Sphere {
private:
	// Affine matrix that tells you how to get from object to world coordinates. 
	Mat4 transform;

public:
	// Create a unit sphere.
	Sphere() { transform = Mat4::identity(); }

	// Create a sphere with a given center and radius. 
	Sphere(Pnt3 center, double radius) {
		transform = Mat4::identity();
		transform.scale(radius);
		transform.translate(center);
	}

	// Return the 4x4 affine matrix that encodes the obj-world space transformation for this object. 
	Mat4 getTransform() const { return transform; }

	// Return the inverse of the transform matrix of this object.
	Mat4 inverse() const;

	// Return the radius of this sphere.
	double radius() const;

	// Return the center of this sphere.
	Pnt3 center() const;

	// Translate this object.
	Sphere& translate(double dx, double dy, double dz);

	// Apply uniform scaling to this object. 
	Sphere& scale(double scalar);

	// Apply non-uniform scaling to this object.
	Sphere& scale(double kx, double ky, double kz);

	// Change the object to a different coordinate system.
	void setCoordSys(Mat4 m);

	// Return the time it takes for a ray to hit this sphere.
	std::vector<double> hit(Ray& r) const;

	// Return the surface normal vector at point p. The surface normal will not be
	// normalized so you will have to normalize it afterwards when doing shading
	// calculations.
	Vec3 normal(const Pnt3& p) const;
};
