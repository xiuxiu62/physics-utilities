#include "math/constants.hpp"
#include "math/vector.hpp"
#include <cmath>

// Vec2
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

// Vec3
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

// Vec4
f32 Vec4::dot(const Vec4 &v) const {
    return x * v.x + y * v.y + z * v.z + w * v.w;
}

f32 Vec4::length_squared() const {
    return x * x + y * y + z * z + w * w;
}

f32 Vec4::length() const {
    return std::sqrt(length_squared());
}

Vec4 Vec4::normalize() const {
    f32 len = length();
    if (len < EPSILON) return Vec4::ZERO(); // Return zero vector if length is nearly zero

    f32 inv_len = 1.0f / len;
    return {x * inv_len, y * inv_len, z * inv_len, w * inv_len};
}

Vec3 project_4d_to_3d(const Vec4 &point, float w_distance) {
    // Simple perspective projection
    float factor = 1.0f / (w_distance - point.w);
    return {point.x * factor, point.y * factor, point.z * factor};
}
