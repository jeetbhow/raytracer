#include "view.h"
#include "object.h"

using namespace std;

int main() {

    // VIEWPORT
    Viewport vp(1, 400, 16.0 / 9.0);

    // MATERIAL
    Material m1(Color(1, 0, 0), 0.05, 0.5, 1.0, 50);
    Material m2(Color(0, 1, 0), 0.05, 0.5, 1.0, 50);

    // OBJECTS
    auto sphereGeometry1 = make_unique<Sphere>(Pnt3(-1, 0, -3), 0.3);
    Object sphere1(move(sphereGeometry1), m1);

    auto sphereGeometry2 = make_unique<Sphere>(Pnt3(0, 0, -10), 2);
    Object sphere2(move(sphereGeometry2), m2);

    vector<Object> objs;
    objs.push_back(move(sphere1));
    objs.push_back(move(sphere2));

    // LIGHTS
    Light l1(Pnt3(-1, 0, 0), Color(1, 1, 1));
    vector<Light> lights;
    lights.push_back(l1);

    // CAMERA
    Camera cam(vp, Pnt3(0, 0, 0), 1);
    double intervalLength = 2;
    double numSteps = 100;
    double stepSize = intervalLength / numSteps;
    for (int i = 0; i < numSteps; ++i) {
        objs[0].geometry->move(-1 + stepSize * i, 0, -3);
        Image img = cam.render(objs, lights);
        img.save(std::format("img{}.png", i));
    }

    return 0;
}