#pragma once
#include "geometry.h"
#include <vector>

// A 3x3 matrix. This matrix is used to store rotation matrices.
class Mat3 {
private:
	static const size_t SIZE = 3;

	// Internal representation of the matrix. 
	double data[3][3];

public: 
	// Create a 3x3 identity matrix and return it. 
	static Mat3 identity();

	// Overload the [] operator to get an element from the matrix using an index. 
	double* operator [](size_t row);

	// Constant version of the [] operator overload. 
	const double* operator [](size_t row) const;

	// Matrix-vector multiplication.
	Vec3 operator *(const Vec3& v) const;

	// Matrix-matrix multiplication. 
	Mat3 operator *(const Mat3& m) const;

	// Transpose the matrix. Transposing a matrix swaps the rows and columns.
	void transpose();

	// Set the elements along the diagonal of the matrix to their reciprocals. This method is
	// most often used to invert a scaling matrix. 
	void setReciprocalDiag();

	// Extract the scaling matrix from this linear transformation. 
	Mat3 extractScaling() const;

	// Return the size of this matrix.
	size_t size();

	// Send a formatted string representation of the matrix to an output stream.
	friend std::ostream& operator <<(std::ostream& os, const Mat3& m);
};

// A 4x4 matrix.
class Mat4 {
private:
	static const size_t SIZE = 4;

	// Internal representation of the matrix.
	double data[SIZE][SIZE];

public:
	// Default constructor creates a 4x4 zero matrix. 
	Mat4();

	// Make a matrix from a 4x4 vector. 
	Mat4(const std::vector<std::vector<double>> arr);

	// Copy constructor
	Mat4(const Mat4& other);

	// Construct an affine matrix from a linear transformation and translation.
	Mat4(const Mat3& linear, Vec3 translation);

	// Computes the inverse of this matrix and return it. 
	Mat4 inverse() const;

	// Extract the linear component of this affine transformation. The linear component corresponds to the 3x3 submatrix 
	// contained in the top left of an affine matrix. 
	Mat3 extractLinear() const;

	// Matrix-point multiplication. Unlike vectors, points can be translated. 
	Pnt3 operator *(const Pnt3& other) const;

	// Matrix-vector multiplcation.
	Vec3 operator *(const Vec3& other) const;

	// Return true if scaling was applied to the matrix and false otherwise. 
	bool isScaled() const;
	
	// Set the translational component of the matrix to the location given by a point. 
	void translate(Pnt3 p);

	// Increment the translational component of the matrix along each axis. 
	void translate(double dx, double dy, double dz);

	// Increase the scaling component of this matrix by a constant factor along each axis. If you want
	// to scale unevenly, then use the version of this method that passes 3 arguments. 
	void scale(double scalar);

	// Increase the scaling component along each axis by factors kx, ky, and kz for the 
	// x, y, and z axes respectively. 
	void scale(double kx, double ky, double kz);

	// Get the length of a particular column.
	double getLength(size_t col) const;

	// Create a 4x4 identity matrix and return it. 
	static Mat4 identity();

	// Matrix-Matrix multiplication.
	Mat4 operator *(const Mat4& other) const;

	// Overload's the [] operator so that you can index into data to get individual elements. 
	double* operator [](size_t row);

	// Constant version of the [] overload. Read-only access.
	const double* operator [](size_t row) const;

	// Fill the top left 3x3 submatrix with the given matrix. 
	void fill(Mat3 m);

	// Send a formatted string representation of the matrix to an output stream.
	friend std::ostream& operator <<(std::ostream& os, const Mat4& m);
};

