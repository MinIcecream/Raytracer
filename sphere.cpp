#include "ray.h"
#include "vec3.h"
#include "shape.h"

class Sphere: public Shape {
    Vec3 center;
    float radius;

public:    
    Vec3 color;
    float emissionStrength;

    Sphere(const Vec3& center, float radius, const Vec3& color, float emissionStrength = 0) : center(center), radius(radius), color(color), emissionStrength(emissionStrength) {}
    bool intersect(const Ray& ray, float& t) const override {
        return true;
    }
};