#include "math/vec3.hpp"
#include "math/constants.hpp"
#include "math/vec2.hpp"
#include "math/vec4.hpp"
#include <cmath>

Vec2 Vec3::truncate() const {
    return {x, y};
}

Vec4 Vec3::extend(f32 w) const {
    return {x, y, z, w};
}

f32 Vec3::dot(const Vec3 &v) const {
    return x * v.x + y * v.y + z * v.z;
}

Vec3 Vec3::cross(const Vec3 &v) const {
    return {
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x,
    };
}

f32 Vec3::length_squared() const {
    return x * x + y * y + z * z;
}

f32 Vec3::length() const {
    return std::sqrt(length_squared());
}

Vec3 Vec3::normalize() const {
    f32 len = length();
    if (len < EPSILON) {
        return Vec3::ZERO(); // Return zero vector if length is nearly zero
    }
    f32 inv_len = 1.0f / len;
    return {x * inv_len, y * inv_len, z * inv_len};
}
