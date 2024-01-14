#include "view.h"
#include "image.h"

#include <iostream>

int main() {
	// Colors
	const Color RED = Color{ 1.0f, 0.0f, 0.0f };
	
	// Points
	const Pnt3 ORIGIN = Pnt3{ 0, 0, 0 };

	// VIEWPORT SETUP
	const double ASPECT_RATIO = 16.0 / 9.0;
	const Viewport vp{ 1.0, 800, ASPECT_RATIO };

	// Create the objects.
	Sphere s{ Pnt3{0, 0, -4}, 1 };
	s.scale(1, 0.5, 1);

	// Create the lights
	Light l{ RED, ORIGIN };

	// Add them to the scene. 
	std::vector<Sphere> objs;
	objs.push_back(s);

	// Add the lights to the scene.
	std::vector<Light> lights;
	lights.push_back(l);

	// Create the camera and render an image.
	Camera camera{ vp, ORIGIN, 1.0};
	Image img = camera.render(objs, lights);
	img.save("img.png");
}