#include "ray.h"
#include "vec3.h"
#include "shape.h"
#include <cmath>

class Sphere: public Shape {
public:    
    Vec3 center;
    float radius;
    Vec3 color;
    float emissionStrength;

    Sphere(const Vec3& center, float radius, const Vec3& color, float emissionStrength = 0) : center(center), radius(radius), color(color), emissionStrength(emissionStrength) {}
    
    bool intersect(const Ray& ray, float& t) const override {
        Vec3 L = ray.origin - center;
        float b = L.dot(ray.direction);
        float c = L.dot(L) - radius * radius;
        float discriminant = b*b - c;

        if (discriminant < 0) return false; // no intersection

        float sqrtDiscriminant = std::sqrt(discriminant);

        float t0 = -b - sqrtDiscriminant;
        float t1 = -b + sqrtDiscriminant;

        // Pick the smallest positive t
        if (t0 > 0) {
            t = t0;
            return true;
        } else if (t1 > 0) {
            t = t1;
            return true;
        }

        return false; // intersection behind ray origin
    }
};