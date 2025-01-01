# Raytracer

## Introduction

This software contains a raytracer and supplementary graphics library built in C++. This was something I mainly built for fun when I was learning about computer graphics. You should *not* try and use this in production. However, you're free to modify it to your heart's content.

## Installation
The repository includes a `CMakeLists.txt` file that will configure CMake to generate the build files for the project. Open a terminal and create a new directory to store the build files. Change to that directory and then run the command ```cmake ..``` to create the build files. If you're on Windows, you'll also need to install MSBuild (or Visual Studio) to build the solution. If you're on Linux you can simply run the Makefile using `make`.


## Usage
![App Screenshot](/images/img.png)

This is some sample code that will setup the scene shown in the above image and render it. 
```cpp
#include <iostream>
#include <vector>
#include <memory>

// Utility functions
inline Material createMaterial(MaterialType type, const Color& color) {
    return Material::from(type, color);
}

inline shared_ptr<Object> createSphere(const Pnt3& center, double radius, const Material& material) {
    return make_shared<Object>(make_shared<Sphere>(center, radius), material);
}

Scene setupScene() {
    // Viewport
    Viewport vp(2, 800, 16.0 / 9.0);

    // Materials
    auto materials = vector<Material>{
        createMaterial(MaterialType::Glass, Color::grey()),
        createMaterial(MaterialType::PolishedMetal, Color::grey()),
        createMaterial(MaterialType::Plastic, Color::white()),
        createMaterial(MaterialType::Plastic, Color(0.9803, 0.501, 0.447)),
        createMaterial(MaterialType::Plastic, Color(0.341, 0.463, 0.831)),
    };

    // Spheres
    vector<shared_ptr<Object>> objs = {
        createSphere(Pnt3(1.0, -0.7, -1.0), 0.7, materials[0]),
        createSphere(Pnt3(-1.0, -0.4, -2.5), 0.9, materials[1]),
        createSphere(Pnt3(0, -500.5, -30), 500, materials[2]),
        createSphere(Pnt3(-501.5, 0, 40), 500, materials[3]),
        createSphere(Pnt3(501.5, 0, 40), 500, materials[4]),
        createSphere(Pnt3(0, 0, -505.5), 500, materials[2]),
    };

    // Lights
    vector<shared_ptr<Light>> lights;
    lights.emplace_back(make_shared<SquareLight>(
        10, Pnt3(0, 2.0, -1.0), Color(1, 1, 1), Vec3(0, -2, 2).normalize(), 1));

    // Camera
    Camera cam(vp, Pnt3(0, 0, 3), 1);

    return Scene(objs, lights, cam);
}

int main() {
    Scene scene = setupScene();
    scene.render("img.png", 2);
    return 0;
}

```
