#include "scene.h"

using namespace std;

int main() {
    // VIEWPORT
    Viewport vp(2, 1280, 16.0 / 9.0);

    // MATERIAL
    auto m1 = Material::from(MaterialType::Glass, Color::red());
    auto m2 = Material::from(MaterialType::PolishedMetal, Color::blue());
    auto m3 = Material::from(MaterialType::Plastic, Color::white());
    auto m4 = Material::from(MaterialType::Plastic, Color(0.9803, 0.501, 0.447));
    auto m5 = Material::from(MaterialType::Plastic, Color(0.341, 0.463, 0.831));

    // GEOMETRY
    auto sphereGeometry1 = make_shared<Sphere>(Pnt3(1.0, -0.7, -1.0), 0.7);
    auto sphereGeometry2 = make_shared<Sphere>(Pnt3(-1.0, -0.4, -2.5), 0.9);
    auto sphereGeometry3 = make_shared<Sphere>(Pnt3(0, -500.5, -30), 500);
    auto sphereGeometry4 = make_shared<Sphere>(Pnt3(-501.5, 0, 40), 500);
    auto sphereGeometry5 = make_shared<Sphere>(Pnt3(501.5, 0, 40), 500);
    auto sphereGeometry6 = make_shared<Sphere>(Pnt3(0, 0, -505.5), 500);

    // OBJECTS
    auto sphere1 = make_shared<Object>(sphereGeometry1, m1);
    auto sphere2 = make_shared<Object>(sphereGeometry2, m2);
    auto sphere3 = make_shared<Object>(sphereGeometry3, m3);
    auto sphere4 = make_shared<Object>(sphereGeometry4, m4);
    auto sphere5 = make_shared<Object>(sphereGeometry5, m5);
    auto sphere6 = make_shared<Object>(sphereGeometry6, m3);

    vector<std::shared_ptr<Object>> objs;
    objs.push_back(move(sphere1));
    objs.push_back(move(sphere2));
    objs.push_back(move(sphere3));
    objs.push_back(move(sphere4));
    objs.push_back(move(sphere5));
    objs.push_back(move(sphere6));

    // LIGHTS
    auto light = make_shared<SquareLight>(10, Pnt3(0, 2.0, -1.0), Color(1, 1, 1),
                                                                                Vec3(0, -2, 2).normalize(), 1);

    vector<std::shared_ptr<Light>> lights;
    lights.push_back(move(light));

    // CAMERA
    Camera cam(vp, Pnt3(0, 0, 3), 1);

    // RENDER
    Scene scene(objs, lights, cam);
    scene.render("img2.png", 2);
}