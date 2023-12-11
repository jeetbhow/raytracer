import geometry;
import <cmath>;
import <stdexcept>;

// Constructor
Vec3::Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

// Calculate the length of the vector
double Vec3::length() const {
  return std::sqrt(x * x + y * y + z * z);
}

// Normalize the vector
Vec3 Vec3::normalize() const {
  double len = length();
  if (len == 0) {
    throw std::runtime_error("Attempt to normalize a zero-length vector.");
  }
  return Vec3(x / len, y / len, z / len);
}

// Calculate the dot product of two vectors
double Vec3::dot(const Vec3& a, const Vec3& b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Calculate the cross product of two vectors
Vec3 Vec3::cross(const Vec3& a, const Vec3& b) {
  return Vec3(
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x
  );
}

// Overload the + operator for vector addition
Vec3 Vec3::operator+(const Vec3& other) const {
  return Vec3(x + other.x, y + other.y, z + other.z);
}

// Overload the - operator for vector subtraction
Vec3 Vec3::operator-(const Vec3& other) const {
  return Vec3(x - other.x, y - other.y, z - other.z);
}

// Overload the * operator for scalar multiplication
Vec3 Vec3::operator*(double scalar) const {
  return Vec3(x * scalar, y * scalar, z * scalar);
}
