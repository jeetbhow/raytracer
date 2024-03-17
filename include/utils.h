#include <random>
#include "object.h"

namespace utils {
static std::mt19937 gen;
static std::uniform_real_distribution<double> distribution(0.0, 1.0);

// Compute the color produced by the Blinn-Phong illumination model for this
// object.
Color phong(const std::shared_ptr<Material> &object, 
    const std::shared_ptr<Light> &light, const Vec3 &L, const Vec3 &V, const Vec3 &N);

double fresnel(double cosTheta, double indexOfRefraction);

// Generate a random number in the given range.
double random(double min, double max);

// Generate a random number between 0 and 1.
double random();
}
