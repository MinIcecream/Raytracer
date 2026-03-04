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

Vec3 trace(const Ray& ray, const std::vector<Sphere>& spheres, int depth = 0) {
    float closestT;
    Sphere* closestSphere = nullptr;
    Vec3 color = Vec3(0, 0, 0); // Background color

    for (const auto& sphere : spheres) {
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
    bool inside = false;
    if (ray.direction.dot(normal) > 0) {
        normal = -normal;
        inside = true;
    }
    if (closestSphere->transparency > 0 && depth < MAX_DEPTH) {
        // reflect ray
        Ray reflectedRay(hitPoint + normal * 0.001f, ray.direction - normal * 2 * ray.direction.dot(normal));
        Vec3 reflectedColor = trace(reflectedRay, spheres, depth + 1);

        // refract ray
        float ior = 1.5f;
        float eta = inside ? ior : 1.0f / ior;

        float cosi = -normal.dot(ray.direction);
        float k = 1 - eta * eta * (1 - cosi * cosi);

        Vec3 refractedColor(0,0,0);

        if (k >= 0) {
            Vec3 refrDir = ray.direction * eta + normal * (eta * cosi - sqrt(k));
            refrDir.normalize();
            Ray refractedRay(hitPoint - normal * 0.001f, refrDir);
            refractedColor = trace(refractedRay, spheres, depth + 1);
        }
        float R0 = pow((1.0f - ior) / (1.0f + ior), 2.0f);
        float R = R0 + (1 - R0) * pow(1 - cosi, 5);
        color = reflectedColor * R + refractedColor * (1 - R);
        return color;
    }

    // Find all emission sources and add their contribution
    for (const auto& s : spheres) {
        if (s.emissionColor .x > 0 || s.emissionColor.y > 0 || s.emissionColor.z > 0) {
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
            color = color + closestSphere->color * s.emissionColor * std::max(0.0f, normal.dot(shadowRayDir));
        }
    }
    return color;
}

void render(std::vector<Sphere>& spheres) {
    int width = 1440;
    int height = 1080;

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
        int r = std::min(255, (int)(color.x * 255));
        int g = std::min(255, (int)(color.y * 255));
        int b = std::min(255, (int)(color.z * 255));
        ofs << r << " " << g << " " << b << "\n";
    }

    ofs.close();
}

int main() {
    std::vector<Sphere> spheres;
    Sphere sphere(Vec3(0, -3, -5), 1.0f, Vec3(1.0, 0, 0));
    Sphere glassSphere(Vec3(0, 3, -5), 1.0f, Vec3(1.0, 0, 0), 0.5f);
    Sphere source(Vec3(2, 2, -5), 0.1f, Vec3(1.0, 0, 0), 0, Vec3(1.0, 1.0, 1.0)); // Light source
    Sphere floor(Vec3(0, -10004, -5), 10000.0f, Vec3(0.8, 0.8, 0.8));
    spheres.push_back(sphere);
    spheres.push_back(source);  
    spheres.push_back(glassSphere);
    spheres.push_back(floor);  
    render(spheres);
    return 0;
}