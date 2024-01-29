#include "scene.h"

#include <algorithm>
#include <format>
#include <random>

#include "utils.h"

const Color Viewport::BACKGROUND_COLOR = Color { 0.5, 0.5, 0.5 };

Vec3 Viewport::dx() const { return Vec3 { width / img->getWidth(), 0, 0 }; }

Vec3 Viewport::dy() const { return Vec3 { 0, height / img->getHeight(), 0 }; }

Pnt3 Viewport::bottomLeft(Pnt3 camPosition, double focalLength) const
{
    return Pnt3(camPosition + Vec3 { -width / 2, -height / 2, -focalLength } + dx() / 2 + dy() / 2);
}

Pnt3 Camera::getPosition() const
{
    return Pnt3(transform[0][3], transform[1][3], transform[2][3]);
}

Viewport Camera::getViewport() const { return viewport; }

double Camera::getFocalLength() const { return focalLength; }

std::vector<Hit*> Scene::castRay(const Ray& r) const
{
    std::vector<Hit*> hits;
    for (size_t i = 0; i < objs.size(); ++i) {
        Hit* hit = objs[i].geometry->hit(r.transformed(inverses[i]));
        hits.push_back(hit);
    }
    return hits;
}

Color Scene::shade(const Pnt3& point, const Vec3& view, const Object& obj,
    const Mat4& inverse) const
{
    Vec3 normal = Geometry::invertNormal(obj.geometry->normal(point), inverse).normalize();
    Pnt3 pntWorld = obj.geometry->getTransform() * point;

    Color finalColor = Color(1, 1, 1) * obj.material.ambient;
    for (const auto& light : lights) {
        auto squareLight = dynamic_cast<SquareLight*>(light.get());
        if (squareLight) {
            std::vector<Pnt3> points = squareLight->getRandomPoints(15);

            Color lightColor = Color(0, 0, 0);
            unsigned blockedRays = 0;
            for (const auto& lightPoint : points) {
                Vec3 pointLightDirection = (lightPoint - pntWorld);
                Ray shadowRay { pntWorld + pointLightDirection / 100,
                    pointLightDirection };
                std::vector<Hit*> hits = castRay(shadowRay);
                auto [hit, index] = getClosestHit(hits);
                if (index != -1) {
                    ++blockedRays;
                } else {
                    double lightDistance = pointLightDirection.length();
                    Vec3 lightVec = (inverse * pointLightDirection).normalize();
                    lightColor += obj.phong(*light, lightDistance,
                        pointLightDirection / lightDistance, view, normal);
                }
            }
            double shadowIntensity = static_cast<double>(blockedRays) / points.size();
            finalColor += lightColor / points.size() * (1 - shadowIntensity);
        }
    }
    finalColor.clamp();
    return finalColor;
}

void Scene::render(const std::string& path, unsigned samples)
{
    // Viewport setup
    Viewport viewport = cam.getViewport();
    Vec3 dx = viewport.dx();
    Vec3 dy = viewport.dy();
    double dxLen = dx.length();
    double dyLen = dy.length();
    Pnt3 bottomLeft = viewport.bottomLeft(cam.getPosition(), cam.getFocalLength());

    // RNG setup
    std::mt19937 gen(5);
    std::uniform_real_distribution<double> distX(-dxLen / 2.0, dxLen / 2.0);
    std::uniform_real_distribution<double> distY(-dyLen / 2.0, dyLen / 2.0);

    // Random offsets that we'll add to the center pixel.
    std::vector<Vec3> offsets(samples);
    std::generate(offsets.begin(), offsets.end(), [&]() { return Vec3(distX(gen),
                                                              distY(gen), 0); });

    // Image setup
    std::shared_ptr<Image> img = viewport.getImg();

    // Shoot a ray through each viewport pixel.
    for (size_t i = 0; i < img->getHeight(); ++i) {
        for (size_t j = 0; j < img->getWidth(); ++j) {
            Color avgColor { 0, 0, 0 };
            for (const auto& offset : offsets) {
                Pnt3 targetPixel = bottomLeft + dx * (double)j + dy * (double)i + offset;
                Vec3 direction = (targetPixel - cam.getPosition()).normalize();
                Ray view { cam.getPosition(), direction };
                std::vector<Hit*> hits = castRay(view);
                auto [hit, minK] = getClosestHit(hits);

                // There was no hit
                if (minK == -1) {
                    avgColor += Viewport::BACKGROUND_COLOR;
                    continue;
                }

                Color color = shade(hit->ray.at(hit->minusT), -hit->ray.direction.normalize(),
                    objs[minK], inverses[minK]);
                avgColor += color;
            }
            avgColor /= samples;
            img->setPixel(img->getHeight() - 1 - i, j, avgColor);
        }
    }
    img->save(path);
}

Image::Image(size_t width, size_t height)
{
    this->width = width;
    this->height = height;
    imgbuf = std::vector<unsigned char>(4 * width * height);
}

Color Image::getPixel(size_t row, size_t col) const
{
    const size_t baseIndex = row * width + col;
    double r = imgbuf[baseIndex] / 255.0;
    double g = imgbuf[baseIndex + 1] / 255.0;
    double b = imgbuf[baseIndex + 2] / 255.0;
    double a = imgbuf[baseIndex + 3] / 255.0;
    return Color { r, g, b, a };
}

void Image::setPixel(size_t row, size_t col, const Color& color)
{
    const size_t baseIndex = 4 * (row * width + col);
    imgbuf[baseIndex] = static_cast<unsigned char>(255.999 * color.r);
    imgbuf[baseIndex + 1] = static_cast<unsigned char>(255.999 * color.g);
    imgbuf[baseIndex + 2] = static_cast<unsigned char>(255.999 * color.b);
    imgbuf[baseIndex + 3] = static_cast<unsigned char>(255.999 * color.a);
}

size_t Image::getWidth() const { return width; }

size_t Image::getHeight() const { return height; }

double Image::aspectRatio() const
{
    return static_cast<double>(width) / static_cast<double>(height);
}

bool Image::save(const std::string& path)
{
    unsigned error = lodepng::encode(path, imgbuf, static_cast<unsigned>(width),
        static_cast<unsigned>(height));
    if (error) {
        std::cout << "Error " << error << ": " << lodepng_error_text(error)
                  << std::endl;
        return false;
    }
    return true;
}

std::ostream& operator<<(std::ostream& os, const Color& p)
{
    os << std::format("<{}, {}, {}>", p.r, p.g, p.b);
    return os;
}