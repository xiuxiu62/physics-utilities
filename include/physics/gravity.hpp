#pragma once

#include "math/rotor.hpp"
#include "math/vector.hpp"
#include "types.hpp"

enum class BodyKind { Kinematic, Dynamic };

struct Dampening {
    float linear;
    float angular;
};

struct Transform2 {
    Vec2 position;
    Vec2 velocity;
    Rot2 rotation;
};

struct Transform3 {
    Vec3 position;
    Vec3 velocity;
    Rot3 rotation;
};

struct Body2 {
    BodyKind kind;
    Transform2 transform;
    float mass;
    Dampening dampening;
};

struct Body3 {
    BodyKind kind;
    Transform3 transform;
    float mass;
    Dampening dampening;
};

void accelerate_rigid_bodies(Body2 *bodies, usize body_count);
void accelerate_rigid_bodies(Body3 *bodies, usize body_count);

void integrate_physics(Body2 *bodies, usize body_count, float dt);
void integrate_physics(Body3 *bodies, usize body_count, float dt);
