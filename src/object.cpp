#include "object.h"

Mat4 Geometry::inverse() const {
	return transform.inverse();
}

Geometry& Geometry::move(double x, double y, double z) {
	transform.setTranslate(x, y, z);
	return *this;
}

Geometry& Geometry::translate(double dx, double dy, double dz) {
	transform.translate(dx, dy, dz);
	return *this;
}

Geometry& Geometry::scale(double scalar) {
	transform.scale(scalar);
	return *this;
}

Geometry& Geometry::scale(double kx, double ky, double kz) {
	transform.scale(kx, ky, kz);
	return *this;
}

void Geometry::setCoordSystem(Mat4 m) {
	transform = m * transform;
}

double Sphere::radius() const {
	return transform[0][0];
}

Pnt3 Sphere::center() const {
	return Pnt3(transform[0][3], transform[1][3], transform[2][3]);
}

std::vector<double> Sphere::hit(const Ray& r) const {
	Vec3 oc = r.origin - Pnt3(0, 0, 0);
	double a = r.direction.dot(r.direction);
	double b = r.direction.dot(oc);
	double c = oc.dot(oc) - 1.0;
	double discriminant = b * b - a * c;

	if (discriminant < 0) {
		return {};
	} else {
		std::vector<double> hits;
		double minusT = (-b - std::sqrt(discriminant)) / a;
		double plusT = (-b + std::sqrt(discriminant)) / a;
		if (minusT > 0) hits.push_back(minusT);
		if (plusT > 0) hits.push_back(plusT);
		return hits;
	}
}

Vec3 Sphere::normal(const Pnt3& p) const {
	return (p - Pnt3(0, 0, 0));
}

Color Object::phong(const Light& light, const Vec3& L, const Vec3& V, const Vec3& N) const {
	Color ambient = Color(1, 1, 1) * material.ambient;
	Color diffuse = light.color * material.color * material.diffuse * std::max(0.0, L.dot(N));

	Vec3 halfway = (V + L).normalize();
	double specularI = std::pow(std::max(0.0, halfway.dot(N)), material.shininess);
	Color specular = light.color * material.specular * specularI;

	return (ambient + diffuse + specular);
}
