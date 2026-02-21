#include <iostream>
#include "sphere.cpp"
#include "vec3.h"
#include "ray.h"
#include <vector>

int main() {
    std::vector<Sphere> shapes;
    Sphere sphere(Vec3(0, 0, 0), 1.0f, Vec3(255, 0, 0));
    shapes.push_back(sphere);
    return 0;
}

void render(std::vector<Sphere>& shapes) {
    int width = 1600;
    int height = 1024;
    for (const auto& sphere : shapes) {
        std::cout<<"Rendering sphere"<<std::endl;
    }
}