#pragma once

#include "common/types.hpp"

struct Vec2;
struct Vec4;

struct Vec3 {
    f32 x, y, z;

    static inline constexpr Vec3 ZERO() {
        return {0, 0, 0};
    }

    static inline constexpr Vec3 ONE() {
        return {1, 1, 1};
    }

    inline constexpr Vec3 operator+(const f32 s) const {
        return {x + s, y + s, z + s};
    }

    inline constexpr Vec3 operator+(const Vec3 &v) const {
        return {x + v.x, y + v.y, z + v.z};
    }

    inline void operator+=(const f32 s) {
        x += s;
        y += s;
        z += s;
    }

    inline void operator+=(const Vec3 &v) {
        x += v.x;
        y += v.y;
        z += v.z;
    }

    inline constexpr Vec3 operator-(const f32 s) const {
        return {x - s, y - s, z - s};
    }

    inline constexpr Vec3 operator-(const Vec3 &v) const {
        return {x - v.x, y - v.y, z - v.z};
    }

    inline void operator-=(const f32 s) {
        x -= s;
        y -= s;
        z -= s;
    }

    inline void operator-=(const Vec3 &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }

    inline constexpr Vec3 operator*(const f32 s) const {
        return {x * s, y * s, z * s};
    }

    inline constexpr Vec3 operator*(const Vec3 &v) const {
        return {x * v.x, y * v.y, z * v.z};
    }

    inline void operator*=(const f32 s) {
        x *= s;
        y *= s;
        z *= s;
    }

    inline void operator*=(const Vec3 &v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
    }

    inline constexpr Vec3 operator/(const f32 s) const {
        return {x / s, y / s, z / s};
    }

    inline constexpr Vec3 operator/(const Vec3 &v) const {
        return {x / v.x, y / v.y, z / v.z};
    }

    inline void operator/=(const f32 s) {
        x /= s;
        y /= s;
        z /= s;
    }

    inline void operator/=(const Vec3 &v) {
        x /= v.x;
        y /= v.y;
        z /= v.z;
    }

    Vec2 truncate() const;
    Vec4 extend(f32 value = 0.0f) const;

    f32 dot(const Vec3 &v) const;
    Vec3 cross(const Vec3 &v) const;
    f32 length_squared() const;
    f32 length() const;
    Vec3 normalize() const;
};
