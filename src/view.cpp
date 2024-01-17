#include "view.h"
#include "light.h"
#include <algorithm>
#include <format>

const Color Viewport::BACKGROUND_COLOR = Color{ 0.5, 0.5, 0.5 };

Vec3 Viewport::dx() const {
	return Vec3{ width / img->getWidth(), 0, 0 };
}

Vec3 Viewport::dy() const {
	return Vec3{ 0, height / img->getHeight(), 0 };
}

Pnt3 Viewport::bottomLeft(Pnt3 camPosition, double focalLength) const {
	return Pnt3(camPosition + Vec3{ -width / 2, -height / 2, -focalLength } + dx() / 2 + dy() / 2);
}

Image& Camera::render(std::vector<Object>& objs, std::vector<Light>& lights) {
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

			// We iterate through each object and find the closest one that the ray intersects. 
			double minT = std::numeric_limits<double>::max();
			size_t minK = -1;

			for (size_t k = 0; k < objs.size(); ++k) {
				Mat4 inverse = inverses[k];

				// Transform the ray into the sphere's coordinate frame.
				Pnt3 originObjSpace = inverse * getPosition();
				Vec3 dirObjSpace = (inverse * direction);
				Ray ray{ originObjSpace, dirObjSpace };

				std::vector<double> hits = objs[k].geometry->hit(ray);

				if (hits.size() > 0 && hits[0] < minT) {
					minT = hits[0];
					minK = k;
				}
			}

			// If the minK hasn't changed, then we didn't get a hit, so continue. 
			if (minK == -1) {
				img->setPixel(img->getHeight() - 1 - i, j, Viewport::BACKGROUND_COLOR);
				continue;
			}

			// Hit. Do shading computation.
			Object& nearestObj = objs[minK];
			Mat4 world = nearestObj.geometry->getTransform();
			Mat4 inverse = inverses[minK];
			
			Vec3 viewVec = (inverse * direction);
			Pnt3 surfacePnt = Ray(inverse * getPosition(), viewVec).at(minT);
			Vec3 normal = nearestObj.geometry->normal(surfacePnt);

			// Transform the surface normal of the object. 
			Mat3 inverseTranspose = inverse.extractLinear();
			inverseTranspose.transpose();
			normal = (inverseTranspose * normal).normalize();

			// Iterate through each light source, and compute the light vector. The light vector is
			// a vector that starts at the surface and points at the light. 
			Color finalColor = Color(0, 0, 0);
			for (auto& light : lights) {
				Pnt3 surfacePntWorld = world * surfacePnt;

				// check if the light vector hits an object when you cast it. If it does,
				// then there's an object between this one and the light so it casts a shadow.
				bool inShadow = false;
				for (size_t l = 0; l < objs.size(); ++l) {
					if (l == minK) {
						continue;
					}
					//std::cout << l << "\n";
					Object& obj = objs[l];
					Mat4 objInverse = objs[l].geometry->inverse();
					Pnt3 surfacePntObj = objInverse * surfacePntWorld;
					Vec3 lightDirectionObj = ((objInverse * light.location) - surfacePntObj).normalize();
					Ray shadowRay(surfacePntObj, lightDirectionObj);
					std::vector<double> hits = obj.geometry->hit(shadowRay);
					if (hits.size() > 0) {
						inShadow = true;
						break;
					}
				}
				
				Vec3 lightVec = (inverse * light.location - surfacePnt).normalize();
				if (!inShadow) {
					finalColor += objs[minK].phong(light, lightVec, -viewVec.normalize(), normal);
				} else {
					finalColor += Color(0.5, 0.5, 0.5) * objs[minK].phong(light, lightVec, -viewVec.normalize(), normal);
				}

			}
			img->setPixel(img->getHeight() - 1 - i, j, finalColor);
		}
	}
	return *img;

}

Pnt3 Camera::getPosition() const {
	return Pnt3(transform[0][3], transform[1][3], transform[2][3]);
}
