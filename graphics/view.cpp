#include "view.h"
#include "light.h"
#include <algorithm>
#include <format>

const Color Viewport::BACKGROUND_COLOR = Color{ 0.0, 0.0, 0.0 };
const Color Viewport::OBJ_COLOR = Color{ 1.0, 0.0, 0.0 };

Vec3 Viewport::dx() const {
	return Vec3{ width / img->getWidth(), 0, 0 };
}

Vec3 Viewport::dy() const {
	return Vec3{ 0, height / img->getHeight(), 0 };
}

Pnt3 Viewport::bottomLeft(Pnt3 camPosition, double focalLength) const {
	return Pnt3(camPosition + Vec3{ -width / 2, -height / 2, -focalLength } + dx() / 2 + dy() / 2);
}

Image& Camera::render(std::vector<Sphere> objs, std::vector<Light> lights, int frame) {
	Pnt3 bottomLeft = viewport.bottomLeft(getPosition(), focalLength);

	// We want to compute the inverse matrix for each obj in the scene.
	std::vector<Mat4> inverses;
	for (const Sphere& obj : objs) {
		inverses.push_back(obj.inverse());
	}

	std::shared_ptr<Image> img = viewport.getImg();

	// Shoot a ray through each viewport pixel.
	for (size_t i = 0; i < img->getHeight(); ++i) {
		for (size_t j = 0; j < img->getWidth(); ++j) {
			Pnt3 targetPixel = bottomLeft + viewport.dx() * (double)j + viewport.dy() * (double)i;
			Vec3 direction = (targetPixel - getPosition()).normalize();
			for (auto& obj : objs) {
				Mat4 inverse = obj.inverse();

				// Transform the ray into the sphere's coordinate frame.
				Pnt3 originObjSpace = inverse * getPosition();
				Vec3 dirObjSpace = (inverse * direction).normalize();
				Ray ray{ originObjSpace, dirObjSpace };

				std::vector<double> hits = obj.hit(ray);

				// No hit. Set pixel to background color and continue. 
				if (hits.size() == 0) {
					img->setPixel(img->getHeight() - 1 - i, j, Viewport::BACKGROUND_COLOR);
					continue;
				}

				// Hit. Do shading computation.
				Pnt3 surfacePnt = ray.at(hits[0]);
				Vec3 normal = (surfacePnt - Pnt3{ 0, 0, 0 }).normalize();

				// This computation transforms the surface normals of the sphere so we can
				// get accurate shading on transformed spheres.
				Mat3 inverseTranspose = inverse.extractLinear();
				inverseTranspose.transpose();
				normal = (inverseTranspose * normal).normalize();

				// We calculate the final value of the pixel by summing the shading calculations on every light source. 
				Color finalColor = Color(0.00, 0.00, 0.00);
				for (auto& light : lights) {
					Vec3 lightDirection = (inverse * light.getLocation() - surfacePnt);
					double lightDistance = lightDirection.length();
					lightDirection = lightDirection / lightDistance;

					double diffuse = std::max(lightDirection.dot(normal), 0.0);
					double attentuation = 1.0 / (1.0 + 0.1 * lightDistance + 0.01 * lightDistance * lightDistance);
					finalColor += (light.getColor() * diffuse * attentuation);
				}
				img->setPixel(img->getHeight() - 1 - i, j, finalColor);
			}
		}
	}
	return *img;
}

Pnt3 Camera::getPosition() const {
	return Pnt3(transform[0][3], transform[1][3], transform[2][3]);
}
