#pragma once

#include "common/types.hpp"

struct Vec3;

struct alignas(16) Vec4 {
    // union {
    // struct {
    f32 x, y, z, w;
    // };
    // __m128 simd;
    // };

    static inline constexpr Vec4 ZERO() {
        return {0, 0, 0, 0};
    }

    static inline constexpr Vec4 ONE() {
        return {1, 1, 1, 1};
    }

    inline constexpr Vec4 operator+(const f32 s) const {
        return {x + s, y + s, z + s, w + s};
    }

    inline constexpr Vec4 operator+(const Vec4 &v) const {
        return {x + v.x, y + v.y, z + v.z, w + v.w};
    }

    inline void operator+=(const f32 s) {
        x += s;
        y += s;
        z += s;
        w += s;
    }

    inline void operator+=(const Vec4 &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.z;
    }

    inline constexpr Vec4 operator-(const f32 s) const {
        return {x - s, y - s, z - s, w - s};
    }

    inline constexpr Vec4 operator-(const Vec4 &v) const {
        return {x - v.x, y - v.y, z - v.z, w - v.w};
    }

    inline void operator-=(const f32 s) {
        x -= s;
        y -= s;
        z -= s;
        w -= s;
    }

    inline void operator-=(const Vec4 &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
    }

    inline constexpr Vec4 operator*(const f32 s) const {
        return {x * s, y * s, z * s, w * s};
    }

    inline constexpr Vec4 operator*(const Vec4 &v) const {
        return {x * v.x, y * v.y, z * v.z, w * v.w};
    }

    inline void operator*=(const f32 s) {
        x *= s;
        y *= s;
        z *= s;
        w *= s;
    }

    inline void operator*=(const Vec4 &v) {

        x *= v.x;
        y *= v.y;
        z *= v.z;
        w *= v.w;
    }

    inline constexpr Vec4 operator/(const f32 s) const {
        return {x / s, y / s, z / s, w / s};
    }

    inline constexpr Vec4 operator/(const Vec4 &v) const {
        return {x / v.x, y / v.y, z / v.z, w / v.w};
    }

    inline void operator/=(const f32 s) {
        x /= s;
        y /= s;
        z /= s;
        w /= s;
    }

    inline void operator/=(const Vec4 &v) {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        w /= v.w;
    }

    Vec3 truncate() const;

    f32 dot(const Vec4 &v) const;
    f32 length_squared() const;
    f32 length() const;
    Vec4 normalize() const;
};
