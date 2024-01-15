#include "geometry.h"

Pnt3 Ray::at(double t) const {
	return origin + direction * t;
}

Pnt3 Pnt3::operator +(const Vec3& other) const {
	return Pnt3(x + other.x, y + other.y, z + other.z);
}

Vec3 Pnt3::operator -(const Pnt3& other) const {
	return Vec3(x - other.x, y - other.y, z - other.z);
}

std::ostream& operator<<(std::ostream& os, const Pnt3& p) {
	os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
	return os;
}

Vec3 Vec3::operator +(const Vec3& other) const {
	return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 Vec3::operator-(const Vec3& other) const {
	return Vec3(x - other.x, y - other.y, z - other.z);
}

Vec3 Vec3::operator -() const {
	return Vec3(-x, -y, -z);
}

Vec3 Vec3::operator *(const double& scalar) const {
	return Vec3(scalar * x, scalar * y, scalar * z);
}

Vec3 Vec3::operator /(const double& scalar) const {
	return Vec3(x / scalar, y / scalar, z / scalar);
}

void Vec3::reciprocal() {
	this->x = 1 / this->x;
	this->y = 1 / this->y;
	this->z = 1 / this->z;
}

void Vec3::negate() {
	this->x = -this->x;
	this->y = -this->y;
	this->z = -this->z;
}

double Vec3::dot(const Vec3& other) const {
	return x * other.x + y * other.y + z * other.z;
}

double Vec3::length() const {
	return sqrt(x * x + y * y + z * z);
}

Vec3 Vec3::normalize() const {
	return *this / length();
}



