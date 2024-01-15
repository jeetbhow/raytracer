#pragma once
#include <iostream>
#include <format>
#include <string>

// Represents a motion or displacement in 3D space. 
struct Vec3 {
	// The components of the vector. 
	double x, y, z;

	// Vector addition and substraction. 
	Vec3 operator +(const Vec3& other) const;
	Vec3 operator -(const Vec3& other) const;

	// Scalar multiplication
	Vec3 operator *(const double& scalar) const;
	Vec3 operator /(const double& scalar) const;
	Vec3 operator -() const;

	// Send a formatted string version of the vector to an ostream. Vectors are denoted by square brackets []. 
	friend std::ostream& operator<<(std::ostream& os, const Vec3& v) {
		os << '[' << v.x << ", " << v.y << ", " << v.z << ']';
		return os;
	}

	std::string toString() const {
		return std::format("[{}, {}, {}]", x, y, z);
	}

	// Take the reciprocal of all of the elements in the vector. 
	void reciprocal();

	// Negate all of the components of this vector.
	void negate();

	// Return the dot product of this vector and another one. 
	double dot(const Vec3& other) const;

	// Return the length of the vector.
	double length() const;

	// Return a unit vector in the direction of this vector. 
	Vec3 normalize() const;
};


// Represents a location in 3D space. 
struct Pnt3 {
	// Coordinates in world-space.
	double x, y, z;

	// (+ operator overload) Add a vector to a point to translate it. 
	Pnt3 operator +(const Vec3& other) const;

	// (- operator overload) Subtract two points to get a vector that represents the displacement between them. 
	Vec3 operator -(const Pnt3& other) const;

	// Send a formatted string version of the point to an output stream. Points are formatted with parentheses (). 
	friend std::ostream& operator <<(std::ostream& os, const Pnt3& p);

	std::string toString() const {
		return std::format("({}, {}, {})", x, y, z);
	}
};


struct Ray {
	Pnt3 origin;
	Vec3 direction;

	// Return the point at t.  
	Pnt3 at(double t) const;
};


