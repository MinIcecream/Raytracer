#include "vec3.h"
#include <cmath>

Vec3::Vec3() : x(0), y(0), z(0) {}

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

Vec3 Vec3::operator-(const Vec3& other) const {
    return Vec3(x - other.x, y - other.y, z - other.z);
}

Vec3 Vec3::normalize() const {
    float length = sqrt(x * x + y * y + z * z);
    if (length > 0) {
        return Vec3(x / length, y / length, z / length);
    }
    return Vec3(0, 0, 0); // Return zero vector if length is zero
}