#pragma once
#include "light.h"

struct Hit {
    Ray ray;
    double minusT;
    double plusT;
    bool isInfront;
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
    Geometry& move(double x, double y, double z);

    // Translate this object.
    Geometry& translate(double dx, double dy, double dz);

    // Apply uniform scaling to this object.
    Geometry& scale(double scalar);

    // Apply non-uniform scaling to this object.
    Geometry& scale(double kx, double ky, double kz);

    // Change the object to a different coordinate system.
    void setCoordSystem(Mat4 m);

    // Get the affine matrix for this geometry.
    Mat4 getTransform() const
    {
        return transform;
    }

    // Get the time it takes for a ray to hit this object.
    virtual Hit* hit(const Ray& r) const = 0;

    // Get the surface normal at point p.
    virtual Vec3 normal(const Pnt3& p) const = 0;

    // Invert a surface normal using the inverse matrix of the object.
    static Vec3 invertNormal(const Vec3& normal, const Mat4& inverse);
};

// A sphere with a radius and center.
class Sphere : public Geometry {
public:
    // Create a unit sphere.
    Sphere()
    {
        transform = Mat4::identity();
    }

    // Create a sphere with a given center and radius.
    Sphere(Pnt3 center, double radius)
    {
        transform = Mat4::identity();
        transform.scale(radius);
        transform.translate(center);
    }

    // Return the radius of this sphere.
    double radius() const;

    // Return the center of this sphere.
    Pnt3 center() const;

    // Get the time it takes for a ray to hit this sphere in object space.
    Hit* hit(const Ray& r) const override;

    // Get the surface normal at point p for this sphere in object space.
    Vec3 normal(const Pnt3& p) const override;
};

// Represents a material in the Blinn-Phong illumination model. The ambient,
// diffuse, and specular coefficients will be real numbers between 0-1.
// Increasing the diffuse component makes the object have a matte-like
// appearance and increasing the specular componenent makes the object look
// metallic. Increasing the shininess makes the object have a smaller, more
// focused specular highlight, whereas decreasing it makes the highlight more
// diffuse.
struct Material {
    Color color;
    double ambient;
    double diffuse;
    double specular;
    double shininess;
    double reflectivity;
    double refractivity;
    double refractiveIndex;

    Material(
        Color color,
        double ambient,
        double diffuse,
        double specular,
        double shininess,
        double reflectivity)
        : color(color)
        , ambient(ambient)
        , diffuse(diffuse)
        , specular(specular)
        , shininess(shininess)
        , reflectivity(reflectivity)
        , refractivity(refractivity)
        , refractiveIndex(refractiveIndex)
    {
    }
};

// A renderable object. An object has a geometry which determines its shape, and
// a material which determines how it interacts with light.
struct Object {
    std::unique_ptr<Geometry> geometry;
    Material material;

    // Create an object.
    Object(std::unique_ptr<Geometry> geometry, const Material& material)
        : geometry(std::move(geometry))
        , material(material)
    {
    }

    // Move constructor
    Object(Object&& other) noexcept
        : geometry(std::move(other.geometry))
        , material(std::move(other.material))
    {
    }

    // Compute the color produced by the Blinn-Phong illumination model for this
    // object.
    Color phong(const Light& light, const double lightDistance, const Vec3& L, const Vec3& V, const Vec3& N) const;
};