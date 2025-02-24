#include "physics/gravity.hpp"
#include "math/constants.hpp"
#include "types.hpp"

const float GRAVITATIONAL_CONSTANT = 6.674e-11;
const float GRAVITATIONAL_FACTOR = 1.0e2;

static Vec2 calculate_acceleration(Vec2 kinematic_position, float kinematic_mass, Vec2 dynamic_position,
                                   float dynamic_mass) {
    Vec2 delta_position = kinematic_position - dynamic_position;
    float distance_squared = delta_position.length_squared();

    // Avoid division by zero and prevent extreme forces at very close distances
    if (distance_squared < EPSILON) {
        return Vec2::ZERO();
    }

    Vec2 force_direction = delta_position.normalize();

    // Special case for massless particles
    if (dynamic_mass < EPSILON) {
        // Approximate light deflection using a simplified model
        // For massless particles, acceleration is independent of the particle's "mass"
        float deflection_factor = GRAVITATIONAL_CONSTANT * kinematic_mass / distance_squared;
        return force_direction * deflection_factor * GRAVITATIONAL_FACTOR;
    }

    // Regular case for particles with mass
    float force_magnitude = (GRAVITATIONAL_CONSTANT * kinematic_mass) / distance_squared;
    Vec2 force = force_direction * force_magnitude;
    return force / dynamic_mass * GRAVITATIONAL_FACTOR;
}

static Vec3 calculate_acceleration(Vec3 kinematic_position, float kinematic_mass, Vec3 dynamic_position,
                                   float dynamic_mass) {
    Vec3 delta_position = kinematic_position - dynamic_position;
    float distance_squared = delta_position.length_squared();

    // Avoid division by zero and prevent extreme forces at very close distances
    if (distance_squared < EPSILON) {
        return Vec3::ZERO();
    }

    Vec3 force_direction = delta_position.normalize();

    // Special case for massless particles
    if (dynamic_mass < EPSILON) {
        // Approximate light deflection using a simplified model
        // For massless particles, acceleration is independent of the particle's "mass"
        float deflection_factor = GRAVITATIONAL_CONSTANT * kinematic_mass / distance_squared;
        return force_direction * deflection_factor * GRAVITATIONAL_FACTOR;
    }

    // Regular case for particles with mass
    float force_magnitude = (GRAVITATIONAL_CONSTANT * kinematic_mass) / distance_squared;
    Vec3 force = force_direction * force_magnitude;
    return force / dynamic_mass * GRAVITATIONAL_FACTOR;
}

void accelerate_rigid_bodies(Body2 *bodies, usize body_count) {
    for (usize i = 0; i < body_count; i++) {
        // Skip kinematic bodies
        if (bodies[i].kind != BodyKind::Dynamic) {
            continue;
        }

        for (usize j = 0; j < body_count; j++) {
            // Skip self-interactions
            if (i == j) {
                continue;
            }

            Vec2 acceleration = calculate_acceleration(bodies[j].transform.position, bodies[j].mass,
                                                       bodies[i].transform.position, bodies[i].mass);

            // Only apply acceleration if it's significant
            if (acceleration.length() > 1e-2) {
                bodies[i].transform.velocity += acceleration;
            }
        }
    }
}

void accelerate_rigid_bodies(Body3 *bodies, usize body_count) {
    for (usize i = 0; i < body_count; i++) {
        // Skip kinematic bodies
        if (bodies[i].kind != BodyKind::Dynamic) {
            continue;
        }

        for (usize j = 0; j < body_count; j++) {
            // Skip self-interactions
            if (i == j) {
                continue;
            }

            Vec3 acceleration = calculate_acceleration(bodies[j].transform.position, bodies[j].mass,
                                                       bodies[i].transform.position, bodies[i].mass);

            // Only apply acceleration if it's significant
            if (acceleration.length() > 1e-2) {
                bodies[i].transform.velocity += acceleration;
            }
        }
    }
}

void integrate_physics(Body2 *bodies, usize body_count, float dt) {
    for (usize i = 0; i < body_count; i++) {
        auto &body = bodies[i];

        body.transform.velocity *= (1.0f - body.dampening.linear * dt);
        body.transform.position += body.transform.velocity * dt;
    }
}

void integrate_physics(Body3 *bodies, usize body_count, float dt) {
    for (usize i = 0; i < body_count; i++) {
        auto &body = bodies[i];

        body.transform.velocity *= (1.0f - body.dampening.linear * dt);
        body.transform.position += body.transform.velocity * dt;
    }
}
