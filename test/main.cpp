#include "view.h"
#include "image.h"

#include <iostream>

int main() {
	
	// Camera + Viewport setup
	const double ASPECT_RATIO = 16.0 / 9.0;
	const size_t IMG_WIDTH = 800;
	const size_t IMG_HEIGHT = static_cast<size_t>(static_cast<double>(IMG_WIDTH) / static_cast<double>(ASPECT_RATIO));
	const double VP_WIDTH = 1.0;
	const double VP_HEIGHT = 1.0 / ASPECT_RATIO;

	Viewport vp{1.0, 1.0 / ASPECT_RATIO};
	Camera camera{ IMG_WIDTH, IMG_HEIGHT, vp };
	Image img = camera.render();
	img.save("img.png");
}