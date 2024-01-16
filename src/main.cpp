#include "view.h"
#include "object.h"

using namespace std;

int main() {

    // VIEWPORT
    Viewport vp(1, 800, 16.0 / 9.0);

    // MATERIAL
    Material m(Color(1, 0, 0), 0.05, 0.5, 1.0, 20);

    // OBJECTS
    auto sphereGeometry1 = make_unique<Sphere>(Pnt3(1, 0, -4), 0.5);
    Object sphere1(move(sphereGeometry1), m);

    auto sphereGeometry2 = make_unique<Sphere>(Pnt3(-1, 0, -4), 0.5);
    Object sphere2(move(sphereGeometry2), m);

    vector<Object> objs;
    objs.push_back(move(sphere1));
    objs.push_back(move(sphere2));

    // LIGHTS
    Light l1(Color(1, 1, 1), Pnt3(0, 0, 0));
    vector<Light> lights;
    lights.push_back(l1);

    // CAMERA
    Camera cam(vp, Pnt3(0, 0, 0), 1);
    Image img = cam.render(objs, lights);
    img.save("img.png");
    return 0;
}