#pragma once
#include "lodepng.h"
#include "object.h"

/**
 * @file scene.h
 * @author Jeet Bhowmik (jeetbhowm@gmail.com)
 * @brief This file contains the definitions for the constructs that are
 * responsible for setting up the scene and rendering it.
 * @version 1.0
 * @date 2024-01-25
 *
 */

class Image {
private:
    std::vector<unsigned char> imgbuf;
    size_t width, height;

public:
    // Create an image with the given dimensions.
    Image(size_t width, size_t height);

    // Get the pixel located at the specified row and col.
    Color getPixel(size_t row, size_t col) const;

    // Set the pixel at the specified row and col.
    void setPixel(size_t row, size_t col, const Color& color);

    // Return the width of the image.
    size_t getWidth() const;

    // Return the height of the image.
    size_t getHeight() const;

    // Get the aspect ratio of the image.
    double aspectRatio() const;

    // Save the image at a desired path
    bool save(const std::string& path);
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
    Viewport(double width, size_t imgWidth, double aspectRatio)
        : width(width)
    {
        this->width = width;
        this->height = width / aspectRatio;

        img = std::make_unique<Image>(
            imgWidth, static_cast<size_t>(std::round(imgWidth / aspectRatio)));
    }
    // Create a viewport by giving the dimensions of the viewport and image.
    Viewport(double vpWidth, double vpHeight, size_t imgWidth, size_t imgHeight)
        : width(vpWidth)
        , height(vpHeight)
    {
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

    // Cast a ray and return the time that it took to hit an object. You must also
    // pass the transformation matrix of the object so that the rays are in the
    // same coordinate space.
    double castRay(Ray& r, const Object& obj, const Mat4& transform) const;

public:
    // Create a camera.
    Camera(const Viewport& viewport, Pnt3 position, double focalLength)
        : viewport(viewport)
        , focalLength(focalLength)
    {
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
    std::vector<Object> objs;

    // Contains the world-inverse matrices for all of the objects in the scene.
    std::vector<Mat4> inverses;

    // Contains all of the lights in the scene.
    std::vector<std::unique_ptr<Light>> lights;

    // The camera is used to render the scene.
    Camera cam;

    // Compute the color at a particular point of an object. The point should be
    // in object-space.
    Color shade(const Pnt3& point,
        const Vec3& view,
        const Object& obj,
        const Mat4& inverse) const;

public:
    // Create a scene with a list of objects, a list of lights, and a camera.
    Scene(std::vector<Object>& objs,
        std::vector<std::unique_ptr<Light>>& lights,
        Camera& cam)
        : objs(std::move(objs))
        , lights(std::move(lights))
        , cam(cam)
    {
        for (const auto& obj : this->objs) {
            this->inverses.push_back(obj.geometry->inverse());
        }
    }

    std::vector<Hit*> castRay(const Ray& r) const;

    // Render the scene and save it as a PNG file.
    void render(const std::string& path, unsigned samples);
};
