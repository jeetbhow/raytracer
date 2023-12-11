import geometry; 

import <cmath>;
import <stdexcept>;

Pnt3::Pnt3(double x, double y, double z) : x(x), y(y), z(z), w(1.0) {}

double Pnt3::distance(const Pnt3& p1, const Pnt3& p2) {
  double dx = p1.x - p2.x;
  double dy = p1.y - p2.y;
  double dz = p1.z - p2.z;
  return std::sqrt(dx * dx + dy * dy + dz * dz);
}

double& Pnt3::operator[](int index) {
  switch (index) {
  case 0: return x;
  case 1: return y;
  case 2: return z;
  case 3: return w;
  default: throw std::out_of_range("Index out of range");
  }
}

const double& Pnt3::operator[](int index) const {
  switch (index) {
  case 0: return x;
  case 1: return y;
  case 2: return z;
  case 3: return w;
  default: throw std::out_of_range("Index out of range");
  }
}

Vec3 Pnt3::operator-(const Pnt3& other) const {
  return Vec3(x - other.x, y - other.y, z - other.z);
}

Pnt3 Pnt3::operator+(const Vec3& vec) const {
  return Pnt3(x + vec.x, y + vec.y, z + vec.z);
}