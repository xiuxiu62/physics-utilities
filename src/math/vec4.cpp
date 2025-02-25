#include "math/vec4.hpp"
#include "math/constants.hpp"
#include "math/vec3.hpp"
#include <cmath>

Vec3 Vec4::truncate() const {
    return {x, y, z};
}

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
