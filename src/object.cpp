#include "object.h"

Mat4 Geometry::inverse() const { return transform.inverse(); }

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

void Geometry::setCoordSystem(Mat4 m) { transform = m * transform; }

Vec3 Geometry::invertNormal(const Vec3& normal, const Mat4& inverse) {
  Mat3 inverseTranspose = inverse.extractLinear();
  inverseTranspose.transpose();
  return (inverseTranspose * normal);
}

double Sphere::radius() const { return transform[0][0]; }

Pnt3 Sphere::center() const {
  return Pnt3(transform[0][3], transform[1][3], transform[2][3]);
}

Hit* Sphere::hit(const Ray& ray) const {
  Vec3 oc = ray.origin - Pnt3(0, 0, 0);
  double a = ray.direction.dot(ray.direction);
  double b = ray.direction.dot(oc);
  double c = oc.dot(oc) - 1.0;
  double discriminant = b * b - a * c;

  if (discriminant < 0) {
    return nullptr;
  } else {
    double minusT = (-b - std::sqrt(discriminant)) / a;
    double plusT = (-b + std::sqrt(discriminant)) / a;
    return new Hit{ray, minusT, plusT, minusT > 0.0 && plusT > 0.0};
  }
}

Vec3 Sphere::normal(const Pnt3& point) const { return (point - Pnt3(0, 0, 0)); }

Color Object::phong(const Light& light, const double lightDistance,
                    const Vec3& L, const Vec3& V, const Vec3& N) const {
  double attenuation = light.intensity / (lightDistance * lightDistance);
  Color diffuse =
      light.color * material.color * material.diffuse * std::max(0.0, L.dot(N));
  Vec3 halfway = (V + L).normalize();
  double specularI =
      std::pow(std::max(0.0, halfway.dot(N)), material.shininess);
  Color specular = light.color * material.specular * specularI;
  Color final = (diffuse + specular) * attenuation;
  return final;
}