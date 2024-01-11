#include "shape.h"

Mat4 Sphere::invert() const {
	return transform.inverse();
}

Sphere::Sphere() {
	transform = Mat4::identity();
}

Sphere::Sphere(Pnt3 center, double radius) {
	transform = Mat4::identity();
	transform.scale(radius, radius, radius);
	transform.translate(center.x, center.y, center.z);
}

double Sphere::radius() const {
	return transform[0][0];
}

Pnt3 Sphere::center() const {
	return Pnt3(transform[0][3], transform[1][3], transform[2][3]);
}

Sphere& Sphere::translate(double dx, double dy, double dz) {
	transform.translate(dx, dy, dz);
	return *this;
}

Sphere& Sphere::scale(double kx, double ky, double kz) {
	transform.scale(kx, ky, kz);
	return *this;
}

std::vector<double> Sphere::hit(Ray& r) const {
	Vec3 oc = r.origin - Pnt3(0, 0, 0);
	double a = r.direction.dot(r.direction);
	double b = r.direction.dot(oc);
	double c = oc.dot(oc) - 1.0;
	double discriminant = b * b - a * c;

	if (discriminant < 0) {
		return {};
	} else {
		double minusT = (-b - std::sqrt(discriminant)) / a;
		double plusT = (-b + std::sqrt(discriminant)) / a;
		return { minusT, plusT };
	}
}

Vec3 Sphere::normal(const Pnt3& p) const {
	return (p - center()).normalize();
}
