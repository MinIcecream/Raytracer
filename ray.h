#pragma once

#include "vec3.h"

class Ray {
    Vec3 origin;
    Vec3 direction;

    public:
        Ray(const Vec3& origin, const Vec3& direction);
};