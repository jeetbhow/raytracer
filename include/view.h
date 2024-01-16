#pragma once
#include "image.h"
#include "object.h"

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

	// Create a viewport with the given width and image width and synchronize their aspect ratios.
	Viewport(double width, size_t imgWidth, double aspectRatio): width(width) {
		this->width = width;
		this->height = width / aspectRatio;

		img = std::make_unique<Image>(
			imgWidth,
			static_cast<size_t>(std::round(imgWidth / aspectRatio))
		);
	}
	// Create a viewport by giving the dimensions of the viewport and image. 
	Viewport(double vpWidth, double vpHeight, size_t imgWidth, size_t imgHeight): width(vpWidth), height(vpHeight) {
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

public: 
	// Create a camera. 
	Camera(const Viewport& viewport, Pnt3 position, double focalLength) : viewport(viewport), focalLength(focalLength) {
		transform = Mat4::identity();
		transform.translate(position);
	}

	// Get the position of the camera in world-space.
	Pnt3 getPosition() const;

	// Create an image of the scene and return it. 
	Image& render(const std::vector<Object>& objs, const std::vector<Light>& lights);
};