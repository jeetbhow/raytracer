#pragma once
#include <format>
#include <iostream>
#include <string>
#include <vector>

// Represents a motion or displacement in 3D space.
struct Vec3
{
  // The components of the vector.
  double x, y, z;

  Vec3 () : x (0), y (0), z (0){};
  Vec3 (double x, double y, double z) : x (x), y (y), z (z) {}

  // Vector addition and substraction.
  Vec3 operator+ (const Vec3 &other) const;
  Vec3 operator- (const Vec3 &other) const;

  // Scalar multiplication
  Vec3 operator* (const double &scalar) const;
  Vec3 operator/ (const double &scalar) const;
  Vec3 operator- () const;

  // Send a formatted string version of the vector to an ostream. Vectors are
  // denoted by square brackets [].
  friend std::ostream &
  operator<< (std::ostream &os, const Vec3 &v)
  {
    os << '[' << v.x << ", " << v.y << ", " << v.z << ']';
    return os;
  }

  std::string
  toString () const
  {
    return std::format ("[{}, {}, {}]", x, y, z);
  }

  // Take the reciprocal of all of the elements in the vector.
  void reciprocal ();

  // Negate all of the components of this vector.
  void negate ();

  // Return the dot product of this vector and another one.
  double dot (const Vec3 &other) const;

  // Return the length of the vector.
  double length () const;

  // Return a unit vector in the direction of this vector.
  Vec3 normalize () const;

  // Compute the dot product of two vectors.
  static double dot (const Vec3 &first, const Vec3 &second);

  // Compute the cross product of two vectors.
  static Vec3 cross (const Vec3 &first, const Vec3 &second);
};

// Represents a location in 3D space.
struct Pnt3
{
  // Coordinates in world-space.
  double x, y, z;

  // Translate this point using a vector. Creates a new point and returns it.
  Pnt3 operator+ (const Vec3 &other) const;

  // Translate this point using a vector.
  void operator+= (const Vec3 &other);

  // Subtract two points and get a vector that represents the displacement
  // between them. Creates a new point and returns it.
  Vec3 operator- (const Pnt3 &other) const;

  // Display this point on an output stream.
  friend std::ostream &operator<< (std::ostream &os, const Pnt3 &p);

  // Convert this point to a string.
  std::string
  toString () const
  {
    return std::format ("({}, {}, {})", x, y, z);
  }
};

// A 3x3 matrix. Used for linear transformations.
class Mat3
{
private:
  static const size_t SIZE = 3;

  // Internal representation of the matrix.
  double data[SIZE][SIZE];

public:
  // Create a 3x3 identity matrix and return it.
  static Mat3 identity ();

  // Overload the [] operator to get an element from the matrix using an index.
  double *operator[] (size_t row);

  // Constant version of the [] operator overload.
  const double *operator[] (size_t row) const;

  // Matrix-vector multiplication.
  Vec3 operator* (const Vec3 &v) const;

  // Matrix-matrix multiplication.
  Mat3 operator* (const Mat3 &m) const;

  // Transpose the matrix. Transposing a matrix swaps the rows and columns.
  void transpose ();

  // Set the elements along the diagonal of the matrix to their reciprocals.
  // This method is most often used to invert a scaling matrix.
  void setReciprocalDiag ();

  // Extract the scaling matrix from this linear transformation.
  Mat3 extractScaling () const;

  // Return the size of this matrix.
  size_t size ();

  // Send a formatted string representation of the matrix to an output stream.
  friend std::ostream &operator<< (std::ostream &os, const Mat3 &m);
};

// A 4x4 matrix. Used for affine transformations.
class Mat4
{
private:
  static const size_t SIZE = 4;

  // Internal representation of the matrix.
  double data[SIZE][SIZE];

public:
  // Default constructor creates a 4x4 zero matrix.
  Mat4 ();

  // Create a matrix from an array.
  Mat4 (const std::vector<std::vector<double> > arr);

  // Copy constructor
  Mat4 (const Mat4 &other);

  // Construct an affine matrix from a 3x3 transformation matrix and a
  // translation vector.
  Mat4 (const Mat3 &linear, Vec3 translation);

  // Computes the inverse of this matrix and return it.
  Mat4 inverse () const;

  // Extract the linear component of this affine transformation. The linear
  // component corresponds to the 3x3 submatrix contained in the top left of an
  // affine matrix.
  Mat3 extractLinear () const;

  // Matrix-point multiplication. Unlike vectors, points can be translated.
  Pnt3 operator* (const Pnt3 &other) const;

  // Matrix-vector multiplcation.
  Vec3 operator* (const Vec3 &other) const;

  // Increment the translational component of the matrix to the location given
  // by a point.
  void translate (Pnt3 p);

  // Increment the translational component of the matrix along each axis.
  void translate (double dx, double dy, double dz);

  // Set the translational component of this matrix to the given configuration.
  void setTranslate (double x, double y, double z);

  // Increase the scaling component of this matrix by a constant factor along
  // each axis. If you want to scale unevenly, then use the version of this
  // method that passes 3 arguments.
  void scale (double scalar);

  // Increase the scaling component along each axis by factors kx, ky, and kz
  // for the x, y, and z axes respectively.
  void scale (double kx, double ky, double kz);

  // Get the length of a particular column.
  double getLength (size_t col) const;

  // Matrix-Matrix multiplication.
  Mat4 operator* (const Mat4 &other) const;

  // Overload's the [] operator so that you can index into data to get
  // individual elements.
  double *operator[] (size_t row);

  // Constant version of the [] overload. Read-only access.
  const double *operator[] (size_t row) const;

  // Fill the top left 3x3 submatrix with the given matrix.
  void fill (Mat3 m);

  // Send a formatted string representation of the matrix to an output stream.
  friend std::ostream &operator<< (std::ostream &os, const Mat4 &m);

  // Create a 4x4 identity matrix and return it.
  static Mat4 identity ();
};

// Represents a ray that shoots out into space in a straight line from some
// origin point.
struct Ray
{
  Pnt3 origin;
  Vec3 direction;

  // Apply an affine transformation to the ray and return the new ray.
  Ray transformed (const Mat4 &m) const;

  // Apply an affine transformation to the ray.
  void transform (const Mat4 &m);

  // Return the point at t.
  Pnt3 at (double t) const;
};
