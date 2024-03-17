#pragma once
#include <cmath>
#include "lodepng.h"
#include "object.h"

class Image {
private:
    std::vector<unsigned char> imgbuf;
    int width, height;

public:
    // Create an image with the given dimensions.
    Image(int width, int height);

    // Get the pixel located at the specified row and col.
    Color getPixel(int row, int col) const;

    // Set the pixel at the specified row and col.
    void setPixel(int row, int col, const Color &color);

    // Return the width of the image.
    int getWidth() const;

    // Return the height of the image.
    int getHeight() const;

    // Get the aspect ratio of the image.
    double aspectRatio() const;

    // Save the image at a desired path
    bool save(const std::string &path);
};

/*
                Represents a virtual image of the scene formed by a camera.
*/
class Viewport {
private:
    double width;
    double height;
    std::shared_ptr<Image> img;

public:
    static const Color BACKGROUND_COLOR;
    static const Color OBJ_COLOR;

    // Create a viewport with the given width and image width and synchronize
    // their aspect ratios.
    Viewport(double width, int imgWidth, double aspectRatio) : width(width) {
        this->width = width;
        this->height = width / aspectRatio;

        img = std::make_unique<Image>(imgWidth, static_cast<int>(std::round(imgWidth / aspectRatio)));
    }
    // Create a viewport by giving the dimensions of the viewport and image.
    Viewport(double vpWidth, double vpHeight, int imgWidth, int imgHeight)
            : width(vpWidth), height(vpHeight) {
        img = std::make_unique<Image>(imgWidth, imgHeight);
    }

    // Get the location of the bottom left pixel of the viewport.
    Pnt3 bottomLeft(Pnt3 camCenter, double focalLength) const;

    // Return a vector that represents the motion required to move across
    // pixel in the horizontal direction.
    Vec3 dx() const;

    // Return a vector that represents the motion required to move across one
    // viewport pixel in the vertical direction.
    Vec3 dy() const;

    // Return a pointer to the image.
    std::shared_ptr<Image> getImg() { return img; }
};

class Camera {
private:
    Viewport viewport;
    Mat4 transform;
    double focalLength;

    // Cast a ray and return the time that it took to hit an object. You must
    // also pass the transformation matrix of the object so that the rays are in
    // the same coordinate space.
    double castRay(Ray &r, const Object &obj, const Mat4 &transform) const;

public:
    // Create a camera.
    Camera(const Viewport &viewport, Pnt3 position, double focalLength)
            : viewport(viewport), focalLength(focalLength) {
        transform = Mat4::identity();
        transform.translate(position);
    }

    // Get the position of the camera in world-space.
    Pnt3 getPosition() const;

    // Get the viewport of the camera.
    Viewport getViewport() const;

    // Get the focal length of the camera.
    double getFocalLength() const;
};

class Scene {
private:
    // Contains all of the objects in the scene.
    std::vector<std::shared_ptr<Object>> objs;

    // Contains all of the lights in the scene.
    std::vector<std::shared_ptr<Light>> lights;

    // The camera is used to render the scene.
    Camera cam;

    // Helper for the shade function. Compute the lighting at a particular point.
    // All arguments must be in world space.
    Color lighting(const Pnt3 &point, const Vec3 &viewDirection,
                 const Vec3 &normal, const std::shared_ptr<Material> &material,
                 unsigned char samples) const;

    // Helper for the shade function. Compute the reflection color at a
    // particular point. All arguments must be in world space.
    Color reflection(const Pnt3 &point, const Vec3 &viewDirection,
                   const Vec3 &normal,
                   const std::shared_ptr<Material> &material,
                   unsigned char depth, const unsigned char maxDepth) const;

    // Helper for the shade function. Compute the transmission color at a
    // particular point. All arguments must be in world space.
    Color transmission(const Pnt3 &point, Vec3 &viewDirection, Vec3 &normal,
                     const std::shared_ptr<Material> &material, const double ki,
                     const double kt, unsigned char depth,
                     const unsigned char maxDepth) const;

    // Compute the color of a hit.
    Color shade(std::unique_ptr<Hit> hit, unsigned char depth) const;

    // The amount to shift a ray by to avoid self-intersection.
    static float BIAS;

public:
    // Create a scene with a list of objects, a list of lights, and a camera.
    Scene(std::vector<std::shared_ptr<Object>> &objs,
                std::vector<std::shared_ptr<Light>> &lights, Camera &cam)
            : objs(std::move(objs)), lights(std::move(lights)), cam(cam) {}

    // Cast a ray onto every object in the scene and return a hit.
    std::optional<std::unique_ptr<Hit>> castRay(Ray &r) const;

    // Render the scene and save it as a PNG file.
    void render(const std::string &path, unsigned samples);
};
