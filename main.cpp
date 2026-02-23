#include <iostream>
#include "sphere.cpp"
#include "vec3.h"
#include "ray.h"
#include <vector>
#include <fstream>

#define MAX_DEPTH 5

Ray getRayToPixel(int x, int y, int width, int height) {
    // Assuming eye is at (0, 0, 0)
    // and image plane is at z = -1

    float aspectRatio = static_cast<float>(width) / height;

    // Bring x and y to range [0, 1]
    float normalizedX = (x + 0.5f) / width;
    float normalizedY = (y + 0.5f) / height;

    // Map to range [-1, 1]
    float mappedX = 2 * normalizedX - 1;
    float mappedY = 1 - 2 * normalizedY;

    mappedX *= aspectRatio; // Adjust for aspect ratio

    Vec3 pixelPos(mappedX, mappedY, -1);
    Vec3 rayOrigin(0,0,0);
    Vec3 rayDir = (pixelPos - rayOrigin).normalize();
    return Ray(rayOrigin, rayDir);
}

Vec3 trace(const Ray& ray, const std::vector<Sphere>& spheres) {
    float closestT;
    Sphere* closestSphere = nullptr;
    Vec3 color = Vec3(0, 0, 0); // Background color

    for (const auto& sphere : spheres) {
        if (sphere.emissionStrength > 0) {
            continue; // Skip light sources
        }

        float t;
        if (sphere.intersect(ray, t)) {
            if (!closestSphere || t < closestT) {
                closestT = t;
                closestSphere = const_cast<Sphere*>(&sphere);
            }
        }
    }

    if (!closestSphere) {
        return color;
    }

    Vec3 hitPoint = ray.origin + ray.direction * closestT;
    Vec3 normal = (hitPoint - closestSphere->center).normalize();
    // Find all emission sources and add their contribution
    for (const auto& s : spheres) {
        if (s.emissionStrength > 0) {
            bool blocked = false;
            Vec3 toLight = s.center - hitPoint;
            Vec3 shadowRayOrigin = hitPoint + normal * 0.001f; // Offset to avoid self-intersection
            Vec3 shadowRayDir = toLight.normalize();
            Ray shadowRay(shadowRayOrigin, shadowRayDir);
            for (const auto& other : spheres) {
                float t;
                if (&other != &s && other.intersect(shadowRay, t)) {
                    if (t < toLight.length()){
                        // Blocks light, does not contribute.
                        blocked = true;
                        break;
                    }
                }
            }
            if (blocked) {
                continue;
            }
            color = color + closestSphere->color * s.emissionStrength * std::max(0.0f, normal.dot(shadowRayDir));
        }
    }
    return color; // Return background color for miss
}

void render(std::vector<Sphere>& spheres) {
    int width = 720;
    int height = 480;

    std::vector<Vec3> image(width * height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Ray ray = getRayToPixel(x, y, width, height);
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
    Sphere sphere(Vec3(0, -3, -5), 1.0f, Vec3(255, 0, 0));
    Sphere source(Vec3(2, 2, -5), 0.1f, Vec3(255, 0, 0), 1.0f);
    Sphere floor(Vec3(0, -10004, -5), 10000.0f, Vec3(255, 0, 0));
    spheres.push_back(sphere);
    spheres.push_back(source);  
    spheres.push_back(floor);  
    render(spheres);
    return 0;
}