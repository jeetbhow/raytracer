#include "object.h"

#include <cmath>

Mat4 Geometry::inverse() const { return transform.inverse(); }

Geometry &Geometry::move(double x, double y, double z) {
    transform.setTranslate(x, y, z);
    return *this;
}

Geometry &Geometry::translate(double dx, double dy, double dz) {
    transform.translate(dx, dy, dz);
    return *this;
}

Geometry &Geometry::scale(double scalar) {
    transform.scale(scalar);
    return *this;
}

Geometry &Geometry::scale(double kx, double ky, double kz) {
    transform.scale(kx, ky, kz);
    return *this;
}

void Geometry::setCoordSystem(Mat4 m) { transform = m * transform; }

Vec3 Geometry::invertNormal(const Vec3 &normal, const Mat4 &inverse) {
    Mat3 inverseTranspose = inverse.extractLinear();
    inverseTranspose.transpose();
    return (inverseTranspose * normal);
}

double Sphere::radius() const { return transform[0][0]; }

Pnt3 Sphere::center() const {
    return Pnt3(transform[0][3], transform[1][3], transform[2][3]);
}

std::optional<std::pair<double, double>> Sphere::hit(const Ray &ray) const {
    Vec3 oc = ray.origin - Pnt3(0, 0, 0);
    double a = ray.direction.dot(ray.direction);
    double b = ray.direction.dot(oc);
    double c = oc.dot(oc) - 1.0;
    double discriminant = b * b - a * c;

    if (discriminant < 0) {
        return std::nullopt;
    } else {
        double minusT = (-b - std::sqrt(discriminant)) / a;
        double plusT = (-b + std::sqrt(discriminant)) / a;
        return std::make_pair(minusT, plusT);
    }
}

Vec3 Sphere::normal(const Pnt3 &point) const { return (point - Pnt3(0, 0, 0)); }

std::shared_ptr<Material> Material::from(const MaterialType type,
                                         const Color &color) {
    switch (type) {
        case MaterialType::Matte:
            return std::make_shared<Material>(color, 0.05, 1.0, 0.0, 30.0, 0.0, 0.0, 1.0);
        case MaterialType::Plastic:
            return std::make_shared<Material>(color, 0.05, 0.4, 0.8, 100.0, 0.0, 0.0, 1.0);
        case MaterialType::PolishedMetal:
            return std::make_shared<Material>(color, 0.05, 0.05, 0.8, 60.0, 0.80, 0.0, 1.0);
        case MaterialType::Glass:
            return std::make_shared<Material>(color, 0.05, 0.0, 0.5, 150.0, 0.40, 0.80, 1.56);
        default:
            return std::make_shared<Material>(color, 0.05, 0.9, 0.1, 30.0, 0.0, 0.0, 1.0);
    }
}