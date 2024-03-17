#include "scene.h"
#include "utils.h"
#include <algorithm>
#include <random>

const Color Viewport::BACKGROUND_COLOR = Color{0.5, 0.5, 0.5};
const Color Viewport::OBJ_COLOR = Color{1, 0, 0};

Vec3 Viewport::dx() const { return Vec3{width / img->getWidth(), 0, 0}; }

Vec3 Viewport::dy() const { return Vec3{0, height / img->getHeight(), 0}; }

Pnt3 Viewport::bottomLeft(Pnt3 camPosition, double focalLength) const {
    return Pnt3(camPosition + Vec3{-width / 2, -height / 2, -focalLength} +
                            dx() / 2 + dy() / 2);
}

Pnt3 Camera::getPosition() const {
    return Pnt3(transform[0][3], transform[1][3], transform[2][3]);
}

Viewport Camera::getViewport() const { return viewport; }

double Camera::getFocalLength() const { return focalLength; }

float Scene::BIAS = 1e-4;

Color Scene::transmission(const Pnt3 &point, Vec3 &viewDirection, Vec3 &normal,
                                                    const std::shared_ptr<Material> &material,
                                                    const double ki, const double kt, unsigned char depth,
                                                    const unsigned char maxDepth) const {
    Color avgColor = Color::black();
    Vec3 refractDirection = Vec3::refract(viewDirection, normal, ki, kt);
    unsigned char samples = 6;

    for (unsigned char i = 0; i < samples; ++i) {
        Vec3 offset = Vec3::randomUnitVector() * 0.10;

        if (Vec3::dot(offset, normal) < 0) {
            offset = -offset;
        }

        Vec3 offsetRefraction = refractDirection + offset;
        Ray refractionRay{point + offsetRefraction * BIAS, offsetRefraction};
        auto result = castRay(refractionRay);
        double reflectance = utils::fresnel(Vec3::dot(viewDirection, -normal), 1.5);

        if (!result.has_value() || depth >= maxDepth) {
            avgColor += Viewport::BACKGROUND_COLOR;
        } else {
            avgColor += shade(std::move(result.value()), depth + 1);
        }
    }

    avgColor /= samples;
    double cosTheta = Vec3::dot(viewDirection, -normal);
    double reflectance = utils::fresnel(cosTheta, 1.5);
    avgColor *= material->color * (1 - reflectance);
    return avgColor;
}

Color Scene::reflection(const Pnt3 &point, const Vec3 &viewDirection,
                                                const Vec3 &normal,
                                                const std::shared_ptr<Material> &material,
                                                unsigned char depth,
                                                const unsigned char maxDepth) const {
    Color color = Color::black();
    Vec3 reflectDirection = Vec3::reflect(viewDirection, normal);

    const unsigned char samples = 6;
    Color avgColor = Color::black();
    for (unsigned char i = 0; i < samples; ++i) {
        Vec3 offset = Vec3::randomUnitVector() * 0.02;

        if (Vec3::dot(offset, normal) < 0) {
            offset = -offset;
        }

        Vec3 sampleReflectionDirection = reflectDirection + offset;
        Ray reflectRay{point + sampleReflectionDirection * Scene::BIAS,
                   sampleReflectionDirection};
        auto result = castRay(reflectRay);

        // add the color that the reflection ray hit to the average
        if (!result.has_value() || depth >= maxDepth) {
            avgColor += Viewport::BACKGROUND_COLOR;
        } else {
            avgColor += shade(std::move(result.value()), depth + 1);
        }
    }

    avgColor /= samples;

    // adjust the color by the material's properties.
    if (material->transparency > 0) {
        double cosTheta = Vec3::dot(viewDirection, -normal);
        double reflectance = utils::fresnel(cosTheta, 1.5);
        avgColor *= material->color * reflectance;
    } else {
        avgColor *= material->color * material->reflectance;
    }

    return avgColor;
}

Color Scene::lighting(const Pnt3 &point, const Vec3 &viewDirection,
                                            const Vec3 &normal,
                                            const std::shared_ptr<Material> &material,
                                            unsigned char samples) const {

    Color totalColor = Color{1, 1, 1} * material->ambient;
    for (const auto &light : lights) {
        auto squareLight = dynamic_cast<SquareLight *>(light.get());
        std::vector<Pnt3> lightPoints = squareLight->getRandomPoints(samples);

        unsigned blockedRays = 0;
        Color lightColor{0, 0, 0};
        for (const auto lightPoint : lightPoints) {
            Vec3 lightDirection = (lightPoint - point);
            double lightDistance = lightDirection.length();
            lightDirection /= lightDistance;
            Ray shadowRay{point + lightDirection * BIAS, lightDirection};
            auto result = castRay(shadowRay);

            // No hit. Light source is obstructed by another object.
            if (result.has_value()) {
                double hitDistance = (result.value()->point - point).length();
                if (hitDistance < lightDistance) {
                    blockedRays++;
                    continue;
                }
            }

            double attenuation = light->intensity / (lightDistance * lightDistance);
            lightColor += utils::phong(material, light, lightDirection,
                                 -viewDirection, normal) *
                                        attenuation;
        }

        double shadowIntensity =
                static_cast<double>(blockedRays) / static_cast<double>(samples);
        totalColor += (lightColor / samples) * (1 - shadowIntensity);
    }

    return totalColor;
}

Color Scene::shade(std::unique_ptr<Hit> hit, unsigned char depth) const {
    Mat4 worldTransform = hit->object->geometry->getTransform();
    Mat4 objTransform = hit->object->geometry->inverse();
    Pnt3 pointWorld = hit->point;
    Vec3 normal = hit->object->geometry->normal(objTransform * pointWorld);
    normal = Geometry::invertNormal(normal, objTransform).normalize();
    Vec3 normalWorld = (worldTransform * normal).normalize();
    Vec3 viewDirection = hit->direction;
    double reflectance = hit->object->material->reflectance;
    double transparency = hit->object->material->transparency;

    Color total = Color::white() * hit->object->material->ambient;

    total += lighting(pointWorld, viewDirection, normalWorld,
                                        hit->object->material, 5);
    if (reflectance > 0 && depth < 4) {
        total += reflection(pointWorld, viewDirection, normalWorld,
                                                hit->object->material, depth, 4) *
             pow(0.3, depth);
    }
    if (transparency > 0 && depth < 4) {
        total += transmission(pointWorld, viewDirection, normalWorld,
                                                    hit->object->material, 1.0, 1.5, depth, 4) *
             pow(0.3, depth);
    }

    total.clamp();
    return total;
}

std::optional<std::unique_ptr<Hit>> Scene::castRay(Ray &ray) const {
    std::shared_ptr<Object> closestObj = nullptr;
    double minMinusT = std::numeric_limits<double>::max();
    double minPlusT = 0;
    for (const auto &obj : objs) {
        Ray objSpaceRay = ray.transformed(obj->geometry->inverse());
        auto hitResult = obj->geometry->hit(objSpaceRay);

        if (!hitResult.has_value()) continue;

        auto [minusT, plusT] = hitResult.value();

        if (minusT < 0) continue;

        if (minusT < minMinusT) {
            minMinusT = minusT;
            minPlusT = plusT;
            closestObj = obj;
        }
    }

    // No hit
    if (!closestObj) {
        return std::nullopt;
    }

    Pnt3 closestPoint = ray.at(minMinusT);
    return std::make_unique<Hit>(closestObj, closestPoint, ray.direction,
                               minMinusT, minPlusT);
}

void Scene::render(const std::string &path, unsigned samples) {
    // Viewport setup
    Viewport viewport = cam.getViewport();
    Vec3 dx = viewport.dx();
    Vec3 dy = viewport.dy();
    double dxLen = dx.length();
    double dyLen = dy.length();
    Pnt3 bottomLeft =
            viewport.bottomLeft(cam.getPosition(), cam.getFocalLength());

    // RNG setup
    std::mt19937 gen(5);
    std::uniform_real_distribution<double> distX(-dxLen / 2.0, dxLen / 2.0);
    std::uniform_real_distribution<double> distY(-dyLen / 2.0, dyLen / 2.0);

    // Random offsets that we'll add to the center pixel.
    std::vector<Vec3> offsets(samples);
    std::generate(offsets.begin(), offsets.end(),
                                [&]() { return Vec3(distX(gen), distY(gen), 0); });
    // Image setup
    std::shared_ptr<Image> img = viewport.getImg();

    // Shoot a ray through each viewport pixel.
    for (int i = 0; i < img->getHeight(); ++i) {
        for (int j = 0; j < img->getWidth(); ++j) {
            Color avgColor{0, 0, 0};
            for (const auto &offset : offsets) {
                Pnt3 targetPixel =
                        bottomLeft + dx * (double)j + dy * (double)i + offset;
                Vec3 direction = (targetPixel - cam.getPosition()).normalize();
                Ray viewRay{cam.getPosition(), direction};
                auto result = castRay(viewRay);

                if (!result.has_value()) {
                    avgColor += Viewport::BACKGROUND_COLOR;
                    continue;
                }

                auto hit = std::move(result.value());
                avgColor += shade(std::move(hit), 0);
            }
            avgColor /= samples;
            avgColor.clamp();
            img->setPixel(img->getHeight() - 1 - i, j, avgColor);
        }
    }
    img->save(path);
}

Image::Image(int width, int height) {
    this->width = width;
    this->height = height;
    imgbuf = std::vector<unsigned char>(4 * width * height);
}

Color Image::getPixel(int row, int col) const {
    const int baseIndex = row * width + col;
    double r = imgbuf[baseIndex] / 255.0;
    double g = imgbuf[baseIndex + 1] / 255.0;
    double b = imgbuf[baseIndex + 2] / 255.0;
    double a = imgbuf[baseIndex + 3] / 255.0;
    return Color{r, g, b, a};
}

void Image::setPixel(int row, int col, const Color &color) {
    const int baseIndex = 4 * (row * width + col);
    imgbuf[baseIndex] = static_cast<unsigned char>(255.0 * color.r);
    imgbuf[baseIndex + 1] = static_cast<unsigned char>(255.0 * color.g);
    imgbuf[baseIndex + 2] = static_cast<unsigned char>(255.0 * color.b);
    imgbuf[baseIndex + 3] = static_cast<unsigned char>(255.0 * color.a);
}

int Image::getWidth() const { return width; }

int Image::getHeight() const { return height; }

double Image::aspectRatio() const {
    return static_cast<double>(width) / static_cast<double>(height);
}

bool Image::save(const std::string &path) {
    unsigned error = lodepng::encode(path, imgbuf, static_cast<unsigned>(width),
                                   static_cast<unsigned>(height));
    if (error) {
        std::cout << "Error " << error << ": " << lodepng_error_text(error)
                            << std::endl;
        return false;
    }
    return true;
}