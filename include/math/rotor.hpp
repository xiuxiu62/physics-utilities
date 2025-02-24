#pragma onceo

#include "vector.hpp"
#include <cmath>

struct Rot2 {
    float scalar, bivector;

    static inline constexpr Rot2 IDENTITY() {
        return {
            .scalar = 1,
            .bivector = 0,
        };
    }

    // constexpr Rot2(f32 scalar, f32 bivector) : scalar(scalar),
    // bivector(bivector) {
    // }

    static inline Rot2 from_angle(float angle) {
        float half_angle = angle * 0.5f;
        return {.scalar = std::cos(half_angle), .bivector = std::sin(half_angle)};
    }

    Vec2 rotate(const Vec2 &v) const {
        float x = v.x * (scalar * scalar - bivector * bivector) + v.y * (-2 * scalar * bivector);
        float y = v.x * (2.0f * scalar * bivector) + v.y * (scalar * scalar - bivector * bivector);
        return {x, y};
    }

    Rot2 operator*(const Rot2 &other) const {
        return {
            .scalar = scalar * other.scalar - bivector * other.bivector,
            .bivector = scalar * other.bivector + bivector * other.scalar,
        };
    }
};

struct Rot3 {
    // In 3D, a rotor has 4 components:
    // - scalar part (1)
    // - bivector parts (e12, e23, e31) - these correspond to the 3 planes of rotation
    float scalar; // scalar part
    float b12;    // xy-plane bivector
    float b23;    // yz-plane bivector
    float b31;    // zx-plane bivector

    static inline constexpr Rot3 IDENTITY() {
        return {
            .scalar = 1.0f,
            .b12 = 0.0f,
            .b23 = 0.0f,
            .b31 = 0.0f,
        };
    }

    // Creates a rotor from an axis and angle
    static inline Rot3 from_axis_angle(const Vec3 &axis, float angle) {
        // Normalize the axis
        float length = std::sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
        if (length < 1e-6f) {
            return IDENTITY();
        }

        Vec3 normalized_axis = {axis.x / length, axis.y / length, axis.z / length};

        // Calculate half angle and its sine
        float half_angle = angle * 0.5f;
        float sin_half = std::sin(half_angle);

        return {.scalar = std::cos(half_angle),
                .b12 = normalized_axis.z * sin_half, // Note: bivector mapping is e12=z, e23=x, e31=y
                .b23 = normalized_axis.x * sin_half,
                .b31 = normalized_axis.y * sin_half};
    }

    // Rotate a vector using the sandwich product: R * v * ~R
    Vec3 rotate(const Vec3 &v) const {
        // This is the expanded form of the sandwich product
        // Compute v' = R * v * ~R where ~R is the reverse of R

        float xx = v.x;
        float yy = v.y;
        float zz = v.z;

        // Compute intermediate values to avoid repetitive calculations
        float ss = scalar * scalar;
        float b12s = b12 * scalar;
        float b23s = b23 * scalar;
        float b31s = b31 * scalar;
        float b12_2 = b12 * b12;
        float b23_2 = b23 * b23;
        float b31_2 = b31 * b31;
        float b12b23 = b12 * b23;
        float b23b31 = b23 * b31;
        float b31b12 = b31 * b12;

        // Calculate rotated coordinates
        float x = xx * (ss + b12_2 - b23_2 - b31_2) + yy * (2.0f * (b31b12 - b23s)) + zz * (2.0f * (b12b23 + b31s));

        float y = xx * (2.0f * (b31b12 + b23s)) + yy * (ss - b12_2 + b23_2 - b31_2) + zz * (2.0f * (b23b31 - b12s));

        float z = xx * (2.0f * (b12b23 - b31s)) + yy * (2.0f * (b23b31 + b12s)) + zz * (ss - b12_2 - b23_2 + b31_2);

        return {x, y, z};
    }

    // Rotor multiplication
    Rot3 operator*(const Rot3 &other) const {
        return {.scalar = scalar * other.scalar - b12 * other.b12 - b23 * other.b23 - b31 * other.b31,
                .b12 = scalar * other.b12 + b12 * other.scalar + b23 * other.b31 - b31 * other.b23,
                .b23 = scalar * other.b23 + b23 * other.scalar + b31 * other.b12 - b12 * other.b31,
                .b31 = scalar * other.b31 + b31 * other.scalar + b12 * other.b23 - b23 * other.b12};
    }

    // Get the reverse (conjugate) of this rotor
    Rot3 reverse() const {
        return {.scalar = scalar, .b12 = -b12, .b23 = -b23, .b31 = -b31};
    }

    // Normalize the rotor
    Rot3 normalize() const {
        float norm_squared = scalar * scalar + b12 * b12 + b23 * b23 + b31 * b31;
        if (norm_squared < 1e-6f) {
            return IDENTITY();
        }

        float inv_norm = 1.0f / std::sqrt(norm_squared);
        return {.scalar = scalar * inv_norm, .b12 = b12 * inv_norm, .b23 = b23 * inv_norm, .b31 = b31 * inv_norm};
    }

    // Linear interpolation between rotors
    static Rot3 slerp(const Rot3 &r1, const Rot3 &r2, float t) {
        // Compute the dot product
        float dot = r1.scalar * r2.scalar + r1.b12 * r2.b12 + r1.b23 * r2.b23 + r1.b31 * r2.b31;

        // Handle the case when rotors are nearly identical
        if (std::abs(dot) > 0.9995f) {
            Rot3 result = {.scalar = r1.scalar + t * (r2.scalar - r1.scalar),
                           .b12 = r1.b12 + t * (r2.b12 - r1.b12),
                           .b23 = r1.b23 + t * (r2.b23 - r1.b23),
                           .b31 = r1.b31 + t * (r2.b31 - r1.b31)};
            return result.normalize();
        }

        // Ensure we take the shortest path
        float sign = (dot < 0.0f) ? -1.0f : 1.0f;
        dot = dot < 0.0f ? -dot : dot;

        // Compute interpolation parameters
        float theta = std::acos(dot);
        float sin_theta = std::sin(theta);

        float scale_r1 = std::sin((1.0f - t) * theta) / sin_theta;
        float scale_r2 = std::sin(t * theta) / sin_theta * sign;

        return {.scalar = scale_r1 * r1.scalar + scale_r2 * r2.scalar,
                .b12 = scale_r1 * r1.b12 + scale_r2 * r2.b12,
                .b23 = scale_r1 * r1.b23 + scale_r2 * r2.b23,
                .b31 = scale_r1 * r1.b31 + scale_r2 * r2.b31};
    }
};
