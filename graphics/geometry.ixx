export module geometry;

import <iostream>;
import <cmath>;

export struct Vec3 {
  double x, y, z;

  Vec3(double x, double y, double z);

  double length() const;
  Vec3 normalize() const;
  static double dot(const Vec3& a, const Vec3& b);
  static Vec3 cross(const Vec3& a, const Vec3& b);

  Vec3 operator+(const Vec3& other) const;
  Vec3 operator-(const Vec3& other) const;
  Vec3 operator*(double scalar) const;
};

export struct Pnt3 {
  double x, y, z, w;

  Pnt3(double x, double y, double z);
  static double distance(const Pnt3& p1, const Pnt3& p2);
  double& operator[](int index);
  const double& operator[](int index) const;
  Vec3 operator-(const Pnt3& other) const;
  Pnt3 operator+(const Vec3& vec) const;
};