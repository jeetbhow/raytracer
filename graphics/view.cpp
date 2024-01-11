#include "view.h"
#include "shape.h"
#include <algorithm>

Camera::Camera(size_t imageWidth, size_t imageHeight, const Viewport& viewport) {
	image = new Image(imageWidth, imageHeight);
	focalLength = 1.0;
	this->viewport = viewport;
}

Image& Camera::render() {
	/*
		The camera and all of the objects have to be transformed into viewing space in order to take the image. 
	*/
	Pnt3 cameraCenter{ 0, 0, 0 };

	const size_t imgWidth = image->getWidth();
	const size_t imgHeight = image->getHeight();
	const double vpWidth = viewport.width;
	const double vpHeight = viewport.height;

	Sphere s{ Pnt3{ 0, 0, -4 }, 1 };

	// Get the dimensions of a viewport pixel. 
	const Vec3 dx{ vpWidth / imgWidth, 0, 0 };
	const Vec3 dy{0, vpHeight / imgHeight, 0};

	// The location of the top left pixel of the viewport.
	Pnt3 topLeft = cameraCenter + Vec3{ -viewport.width / 2, -viewport.height / 2, -focalLength } + dx / 2 + dy / 2;
	Mat4 inverse = s.invert();
	
	// Shoot a ray through each viewport pixel. 
	for (size_t i = 0; i < imgHeight; ++i) {
		for (size_t j = 0; j < imgWidth; ++j) {
			Pnt3 pixelLocation = topLeft + dx * (double) j + dy * (double) i;
			Vec3 rayDirection = (pixelLocation - cameraCenter).normalize();

			Pnt3 originObjSpace = inverse * cameraCenter;
			Vec3 rayDirObjSpace = (inverse * rayDirection).normalize();
			Ray ray{ originObjSpace, rayDirObjSpace };
			
			std::vector<double> hits = s.hit(ray);

			// If there's no hit, then set the color to black and continue.
			if (hits.size() == 0) {
				image->setPixel(i, j, Pixel{ 0.0f, 0.0f, 0.0f });
				continue;
			}

			// If there is a hit then we want to do our shading computations.
			Vec3 normal = ray.at(hits[0]) - Pnt3(0.0, 0.0, 0.0);

			// This computation transforms the surface normals of the sphere so we can get accurate shading on
			// transformed spheres.
			Mat3 inverseTranspose = inverse.extractLinear();
			inverseTranspose.transpose();
			normal = (inverseTranspose * normal).normalize();

			double diffuse = -rayDirection.dot(normal);
			Pixel color{ std::max(0.0f, static_cast<float>(1.0 * diffuse)), 0.0f, 0.0f };
			image->setPixel(i, j, color);
		}
	}

	return *image;
}