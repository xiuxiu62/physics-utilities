#pragma once

#include "common/types.hpp"

struct Vec3;

struct Vec2 {
    f32 x, y;

    static inline constexpr Vec2 ZERO() {
        return Vec2{.x = 0, .y = 0};
    }

    static inline constexpr Vec2 ONE() {
        return Vec2{.x = 1, .y = 1};
    }

    inline Vec2 operator+(const f32 s) const {
        return {x + s, y + s};
    }

    inline constexpr Vec2 operator+(const Vec2 &v) const {
        return {x + v.x, y + v.y};
    }

    inline void operator+=(const f32 s) {
        x += s;
        y += s;
    }

    inline void operator+=(const Vec2 &v) {
        x += v.x;
        y += v.y;
    }

    inline constexpr Vec2 operator-(const f32 s) const {
        return {x - s, y - s};
    }

    inline constexpr Vec2 operator-(const Vec2 &v) const {
        return {x - v.x, y - v.y};
    }

    inline void operator-=(const f32 s) {
        x -= s;
        y -= s;
    }

    inline void operator-=(const Vec2 &v) {
        x -= v.x;
        y -= v.y;
    }

    inline constexpr Vec2 operator*(const f32 s) const {
        return {x * s, y * s};
    }

    inline constexpr Vec2 operator*(const Vec2 &v) const {
        return {x * v.x, y * v.y};
    }

    inline void operator*=(const f32 s) {
        x *= s;
        y *= s;
    }

    inline void operator*=(const Vec2 &v) {
        x *= v.x;
        y *= v.y;
    }

    inline constexpr Vec2 operator/(const f32 s) const {
        return {x / s, y / s};
    }

    inline constexpr Vec2 operator/(const Vec2 &v) const {
        return {x / v.x, y / v.y};
    }

    inline void operator/=(const f32 s) {
        x /= s;
        y /= s;
    }

    inline void operator/=(const Vec2 &v) {
        x /= v.x;
        y /= v.y;
    }

    Vec3 extend(f32 value = 0.0f) const;

    f32 dot(const Vec2 &v) const;
    f32 perp_dot(const Vec2 &v) const;
    f32 length_squared() const;
    f32 length() const;
    Vec2 normalize() const;
};

// struct Transform {
//     Vec3 translation;
//     Vec3 scale;
//     Rot3 rotation;
// };
