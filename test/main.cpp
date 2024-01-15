#include "view.h"
#include "image.h"
#include <iostream>

using namespace std;

int main() {

	Color RED(1.0, 0.1, 0.1);
	Color WHITE(1.0, 1.0, 1.0);

	// VIEWPORT
	const double ASPECT_RATIO = 16.0 / 9.0;
	const Viewport vp(1.0, 1280, ASPECT_RATIO);

	// OBJECTS
	unique_ptr<Geometry> sphereGeometry = make_unique<Sphere>(Pnt3(0, 0, -4), 1);
	Object sphere(move(sphereGeometry), Material(RED, 0.05, 0.5, 0.5, 20));
 
	vector<Object> objs;
	objs.push_back(move(sphere));
	
	// LIGHTS
	Light whiteLight(WHITE, Pnt3(-5, 3, 0));
	vector<Light> lights{ whiteLight };

	// CAMERA
	Camera cam(vp, Pnt3(0, 0, 0), 1);
	Image img = cam.render(objs, lights);
	img.save("img.png");
}