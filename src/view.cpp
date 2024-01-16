#include "view.h"
#include "light.h"
#include <algorithm>
#include <format>

const Color Viewport::BACKGROUND_COLOR = Color{ 0.5, 0.5, 0.5 };
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

Image& Camera::render(const std::vector<Object>& objs, const std::vector<Light>& lights) {
	Pnt3 bottomLeft = viewport.bottomLeft(getPosition(), focalLength);

	// We want to compute the inverse matrix for each obj in the scene.
	std::vector<Mat4> inverses;
	for (const auto& obj : objs) {
		inverses.push_back(obj.geometry->inverse());
;	}

	std::shared_ptr<Image> img = viewport.getImg();

	// Shoot a ray through each viewport pixel.
	for (size_t i = 0; i < img->getHeight(); ++i) {
		for (size_t j = 0; j < img->getWidth(); ++j) {
			Pnt3 targetPixel = bottomLeft + viewport.dx() * (double)j + viewport.dy() * (double)i;
			Vec3 direction = (targetPixel - getPosition()).normalize();
			
			for (size_t k = 0; k < objs.size(); ++k) {
				Mat4 inverse = inverses[k];

				// Transform the ray into the sphere's coordinate frame.
				Pnt3 originObjSpace = inverse * getPosition();
				Vec3 dirObjSpace = (inverse * direction).normalize();
				Ray ray{ originObjSpace, dirObjSpace };

				std::vector<double> hits = objs[k].geometry->hit(ray);

				// No hit. Set pixel to background color and continue. 
				if (hits.size() == 0) {
					img->setPixel(img->getHeight() - 1 - i, j, Viewport::BACKGROUND_COLOR);
					continue;
				}

				// Hit. Do shading computation.
				Pnt3 surfacePnt = ray.at(hits[0]);
				Vec3 normal = objs[k].geometry->normal(surfacePnt);

				// Transform the surface normal of the object. 
				Mat3 inverseTranspose = inverse.extractLinear();
				inverseTranspose.transpose();
				normal = (inverseTranspose * normal).normalize();

				// We calculate the final value of the pixel by summing the shading calculations on every light source. 
				Color finalColor = Color(0.00, 0.00, 0.00);
				for (auto& light : lights) {
					Vec3 lightDirection = (inverse * light.getLocation() - surfacePnt).normalize();
					finalColor += objs[k].phong(light, lightDirection, -dirObjSpace, normal);
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
