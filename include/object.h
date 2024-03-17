#pragma once
#include <memory>
#include <optional>
#include "light.h"

class Object;

struct Hit {
    std::shared_ptr<Object> object;
    Pnt3 point;
    Vec3 direction;
    double minusT;
    double plusT;
};

// Determines the shape of an object.
class Geometry {
protected:
    // An affine matrix that tells you how to get from object to world space.
    Mat4 transform;

public:
    // Return the inverse of the transform matrix of this object.
    Mat4 inverse() const;

    // Move this object to the given location.
    Geometry &move(double x, double y, double z);

    // Translate this object.
    Geometry &translate(double dx, double dy, double dz);

    // Apply uniform scaling to this object.
    Geometry &scale(double scalar);

    // Apply non-uniform scaling to this object.
    Geometry &scale(double kx, double ky, double kz);

    // Change the object to a different coordinate system.
    void setCoordSystem(Mat4 m);

    // Get the affine matrix for this geometry.
    Mat4 getTransform() const { return transform; }

    // Get the time it takes for a ray to hit this object.
    virtual std::optional<std::pair<double, double>> hit(const Ray &r) const = 0;

    // Get the surface normal at point p.
    virtual Vec3 normal(const Pnt3 &p) const = 0;

    // Invert a surface normal using the inverse matrix of the object.
    static Vec3 invertNormal(const Vec3 &normal, const Mat4 &inverse);
};

// A sphere with a radius and center.
class Sphere : public Geometry {
public:
    // Create a unit sphere.
    Sphere() { transform = Mat4::identity(); }

    // Create a sphere with a given center and radius.
    Sphere(Pnt3 center, double radius) {
        transform = Mat4::identity();
        transform.scale(radius);
        transform.translate(center);
    }

    // Return the radius of this sphere.
    double radius() const;

    // Return the center of this sphere.
    Pnt3 center() const;

    // Get the time it takes for a ray to hit this sphere in object space.
    std::optional<std::pair<double, double>> hit(const Ray &r) const override;

    // Get the surface normal at point p for this sphere in object space.
    Vec3 normal(const Pnt3 &p) const override;
};

// Represents a material in the Blinn-Phong illumination model. The ambient,
// diffuse, and specular coefficients will be real numbers between 0-1.
// Increasing the diffuse component makes the object have a matte-like
// appearance and increasing the specular componenent makes the object look
// metallic. Increasing the shininess makes the object have a smaller, more
// focused specular highlight, whereas decreasing it makes the highlight more
// diffuse.

enum MaterialType {
    Matte,
    Plastic,
    PolishedMetal,
    Glass,
};

struct Material {
    Color color;
    double ambient;
    double diffuse;
    double specular;
    double shininess;
    double reflectance;
    double transparency;

    Material(Color color, double ambient, double diffuse, double specular,
           double shininess, double reflectance, double transparency,
           double refractiveIndex)
            : color(color), ambient(ambient), diffuse(diffuse), specular(specular),
                shininess(shininess), reflectance(reflectance),
                transparency(transparency) {}

    static std::shared_ptr<Material> from(const MaterialType type,
                                                                                const Color &color);
};

// A renderable object. An object has a geometry which determines its shape,
// and a material which determines how it interacts with light.
struct Object {
    std::shared_ptr<Geometry> geometry;
    std::shared_ptr<Material> material;

    // Create an object.
    Object(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material)
            : geometry(geometry), material(material) {}
};
