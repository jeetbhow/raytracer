#include "scene.h"

using namespace std;

int main() {

    // VIEWPORT
    Viewport vp(1, 800, 16.0 / 9.0);

    // MATERIAL
    Material m1(Color(1, 0, 0), 0.05, 0.5, 0.5, 30);
    Material m2(Color(0, 1, 0), 0.05, 0.5, 0.5, 30);

    // OBJECTS
    auto sphereGeometry1 = make_unique<Sphere>(Pnt3(-2, 0, -10), 0.5);
    Object sphere1(move(sphereGeometry1), m1);

    auto sphereGeometry2 = make_unique<Sphere>(Pnt3(0, 0, -20), 2);
    Object sphere2(move(sphereGeometry2), m2);

    vector<Object> objs;
    objs.push_back(move(sphere1));
    objs.push_back(move(sphere2));

    // LIGHTS
    Light l1(Pnt3(-4, 0, 0), Color(1, 1, 1));
    vector<Light> lights;
    lights.push_back(l1);

    // CAMERA
    Camera cam(vp, Pnt3(0, 0, 0), 1);    

    // RENDER
    Scene scene(objs, lights, cam);
    scene.render("img.png");
}