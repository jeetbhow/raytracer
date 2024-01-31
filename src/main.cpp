#include "scene.h"

using namespace std;

int main()
{
    // VIEWPORT
    Viewport vp(2, 600, 16.0 / 9.0);

    // MATERIAL
    Material m1(Color(1, 0, 0), 0.05, 0.1, 0.8, 30, 1.0);
    Material m2(Color(0, 0, 1), 0.05, 0.5, 0.5, 30, 0.0);
    Material m3(Color(0.5, 0.5, 0), 0.05, 0.5, 0.5, 20, 0.1);

    // OBJECTS
    auto sphereGeometry1 = make_unique<Sphere>(Pnt3(0.5, 0.25, -1.3), 0.8);
    Object sphere1(move(sphereGeometry1), m1);

    auto sphereGeometry2 = make_unique<Sphere>(Pnt3(-0.5, -0.3, -0.3), 0.3);
    Object sphere2(move(sphereGeometry2), m2);

    auto sphereGeometry3 = make_unique<Sphere>(Pnt3(0, -20.5, -2), 20);
    Object sphere3(move(sphereGeometry3), m3);

    vector<Object> objs;
    objs.push_back(move(sphere1));
    objs.push_back(move(sphere2));
    objs.push_back(move(sphere3));

    // LIGHTS
    auto light = make_unique<SquareLight>(8, Pnt3(-2, 2, 1), Color(1, 1, 1),
        Vec3(2, -2, -1).normalize(), 1);

    vector<std::unique_ptr<Light>> lights;
    lights.push_back(std::move(light));

    // CAMERA
    Camera cam(vp, Pnt3(0, 0, 2), 1);

    // RENDER
    Scene scene(objs, lights, cam);
    scene.render("img1.png", 2);
}