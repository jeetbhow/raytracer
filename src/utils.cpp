#include "utils.h"

Color utils::phong(const std::shared_ptr<Material> &material,
                   const std::shared_ptr<Light> &light, const Vec3 &L,
                   const Vec3 &V, const Vec3 &N) {
  Color diffuse = light->color * material->color * material->diffuse *
                  std::max(0.0, L.dot(N));
  Vec3 halfway = (V + L).normalize();
  double specularI =
      std::pow(std::max(0.0, halfway.dot(N)), material->shininess);
  Color specular = light->color * material->specular * specularI;
  Color final = (diffuse + specular);
  return final;
}

double utils::fresnel(double cosTheta, double ior) {
  float r0 = (1 - ior) / (1 + ior);
  r0 = r0 * r0;
  return r0 + (1 - r0) * std::pow(1 - cosTheta, 5);
}

double utils::random(double min, double max) {
  static std::uniform_real_distribution<double> distribution(min, max);
  return distribution(gen);
}

double utils::random() { return utils::distribution(gen); }