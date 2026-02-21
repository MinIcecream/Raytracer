#include "ray.h"

class Shape {
public:
    virtual bool intersect(const Ray& ray, float& t) const = 0;
    virtual ~Shape() = default;
};