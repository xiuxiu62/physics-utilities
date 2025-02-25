#include "math/vec2.hpp"
#include "math/constants.hpp"
#include "math/vec3.hpp"
#include <cmath>

Vec3 Vec2::extend(f32 z) const {
    return {x, y, z};
}

f32 Vec2::dot(const Vec2 &v) const {
    return x * v.x + y * v.y;
}

f32 Vec2::perp_dot(const Vec2 &v) const {
    return x * v.y - y * v.x;
}

f32 Vec2::length_squared() const {
    return x * x + y * y;
}

f32 Vec2::length() const {
    return std::sqrt(length_squared());
}

Vec2 Vec2::normalize() const {
    f32 len = length();
    if (len < EPSILON) {
        return Vec2::ZERO(); // Return zero vector if length is nearly zero
    }
    f32 inv_len = 1.0f / len;
    return {x * inv_len, y * inv_len};
}
