#pragma once

#include "common/types.hpp"
#include "math/vec2.hpp"
#include "math/vec3.hpp"

// 2D
struct Triangle {
    Vec2 vertices[3];
};

struct Rectangle {
    Vec2 half_size;
};

struct Circle {
    f32 radius;
};

struct Ellipse {
    Vec2 half_size;
};

template <usize N> struct Polygon {
    Vec2 vertices[N];
};

struct Capsule2D {
    f32 radius;
    f32 half_length;
};

// 3D
struct Cuboid {
    Vec3 half_size;
};

struct Sphere {
    f32 radius;
};

struct Cylinder {
    f32 radius;
    f32 half_height;
};

struct ConicalFrustum {
    f32 radius_top;
    f32 radius_bottom;
    f32 height;
};
