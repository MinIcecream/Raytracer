#include "ray.h"
#include "vec3.h"
#include "shape.h"

class Sphere: public Shape {
    Vec3 center;
    float radius;
    Vec3 color;

public:
    Sphere(const Vec3& center, float radius, const Vec3& color) : center(center), radius(radius), color(color) {}
    bool intersect(const Ray& ray, float& t) const override {
        return true;
    }
};