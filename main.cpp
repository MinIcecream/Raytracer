#include <iostream>
#include "sphere.cpp"
#include "vec3.h"
#include "ray.h"
#include <vector>
#include <fstream>

#define MAX_DEPTH 5

Vec3 getRayDirectionToPixel(int x, int y, int width, int height) {
    // Assuming eye is at (0, 0, 0)
    // and image plane is at z = -1

    float aspectRatio = static_cast<float>(width) / height;
}

Vec3 trace(const Ray& ray, const std::vector<Sphere>& spheres) {
    for (const auto& sphere : spheres) {
        float t;
        if (sphere.intersect(ray, t)) {
            return sphere.color; // Return red color for hit
        }
    }
    return Vec3(255, 0, 0); // Return red color for miss
}

void render(std::vector<Sphere>& spheres) {
    int width = 720;
    int height = 480;

    std::vector<Vec3> image(width * height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Vec3 rayDirection = getRayDirectionToPixel(x, y, width, height);
            Ray ray(Vec3(0, 0, 0), rayDirection);
            image[y * width + x] = trace(ray, spheres);
        }
    }

    // Write framebuffer to file
    std::ofstream ofs("image.ppm");
    ofs << "P3\n" << width << " " << height << "\n255\n";
    for (const auto& color : image) {
        int r = std::min(255, (int)color.x);
        int g = std::min(255, (int)color.y);
        int b = std::min(255, (int)color.z);
        ofs << r << " " << g << " " << b << "\n";
    }

    ofs.close();
}

int main() {
    std::vector<Sphere> spheres;
    Sphere sphere(Vec3(0, 0, 0), 1.0f, Vec3(255, 0, 0));
    spheres.push_back(sphere);
    render(spheres);
    return 0;
}