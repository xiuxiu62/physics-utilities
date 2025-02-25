#include "physics/gravity.hpp"
#include <iostream>

const float dt = 0.016f; // ~60fps
const int NUM_STEPS = 1000000;

// Print helper functions
void print_body_state(const Body2 &body, const char *label) {
    std::cout << label << " - Pos: (" << body.transform.position.x << ", " << body.transform.position.y << "), Vel: ("
              << body.transform.velocity.x << ", " << body.transform.velocity.y << "), Mass: " << body.mass
              << std::endl;
}

void print_body_state(const Body3 &body, const char *label) {
    std::cout << label << " - Pos: (" << body.transform.position.x << ", " << body.transform.position.y << ", "
              << body.transform.position.z << "), Vel: (" << body.transform.velocity.x << ", "
              << body.transform.velocity.y << ", " << body.transform.velocity.z << "), Mass: " << body.mass
              << std::endl;
}

// Test 2D physics with massive and massless bodies
void test_2d_physics() {
    std::cout << "\n=== Testing 2D Physics ===\n" << std::endl;

    const int NUM_BODIES = 3;
    Body2 bodies[NUM_BODIES];

    // Set up a massive "sun" at center
    bodies[0].transform.position = {0.0f, 0.0f};
    bodies[0].transform.velocity = {0.0f, 0.0f};
    bodies[0].mass = 1.0e10f; // Large mass
    bodies[0].kind = BodyKind::Dynamic;
    bodies[0].dampening.linear = 0.0f; // No dampening for sun

    // Set up a massive "planet"
    bodies[1].transform.position = {100.0f, 0.0f};
    bodies[1].transform.velocity = {0.0f, 10.0f}; // Initial velocity perpendicular to sun
    bodies[1].mass = 1.0e4f;
    bodies[1].kind = BodyKind::Dynamic;
    bodies[1].dampening.linear = 0.0f;

    // Set up a massless "photon"
    bodies[2].transform.position = {-100.0f, 50.0f};
    bodies[2].transform.velocity = {20.0f, 0.0f}; // Moving from left to right
    bodies[2].mass = 0.0f;                        // Massless
    bodies[2].kind = BodyKind::Dynamic;
    bodies[2].dampening.linear = 0.0f;

    // Run simulation for several steps
    std::cout << "Initial state:" << std::endl;
    print_body_state(bodies[0], "Sun");
    print_body_state(bodies[1], "Planet");
    print_body_state(bodies[2], "Photon");

    for (int step = 0; step < NUM_STEPS; step++) {
        accelerate_rigid_bodies(bodies, NUM_BODIES);
        integrate_physics(bodies, NUM_BODIES, dt);

        // Print state every 50 steps
        // if (step % 50 == 0) {
        //     std::cout << "\nStep " << step << ":" << std::endl;
        //     print_body_state(bodies[0], "Sun");
        //     print_body_state(bodies[1], "Planet");
        //     print_body_state(bodies[2], "Photon");
        // }
    }

    std::cout << "\nFinal state:" << std::endl;
    print_body_state(bodies[0], "Sun");
    print_body_state(bodies[1], "Planet");
    print_body_state(bodies[2], "Photon");
}

// Test 3D physics with massive and massless bodies
void test_3d_physics() {
    std::cout << "\n=== Testing 3D Physics ===\n" << std::endl;

    const int NUM_BODIES = 3;
    Body3 bodies[NUM_BODIES];

    // Set up a massive "sun" at center
    bodies[0].transform.position = {0.0f, 0.0f, 0.0f};
    bodies[0].transform.velocity = {0.0f, 0.0f, 0.0f};
    bodies[0].mass = 1.0e10f; // Large mass
    bodies[0].kind = BodyKind::Dynamic;
    bodies[0].dampening.linear = 0.0f; // No dampening for sun

    // Set up a massive "planet"
    bodies[1].transform.position = {100.0f, 0.0f, 0.0f};
    bodies[1].transform.velocity = {0.0f, 10.0f, 2.0f}; // Initial velocity with z component
    bodies[1].mass = 1.0e4f;
    bodies[1].kind = BodyKind::Dynamic;
    bodies[1].dampening.linear = 0.0f;

    // Set up a massless "photon"
    bodies[2].transform.position = {-100.0f, 50.0f, 25.0f};
    bodies[2].transform.velocity = {20.0f, 0.0f, -5.0f}; // Moving with z component
    bodies[2].mass = 0.0f;                               // Massless
    bodies[2].kind = BodyKind::Dynamic;
    bodies[2].dampening.linear = 0.0f;

    // Run simulation for several steps
    std::cout << "Initial state:" << std::endl;
    print_body_state(bodies[0], "Sun");
    print_body_state(bodies[1], "Planet");
    print_body_state(bodies[2], "Photon");

    for (int step = 0; step < NUM_STEPS; step++) {
        accelerate_rigid_bodies(bodies, NUM_BODIES);
        integrate_physics(bodies, NUM_BODIES, dt);

        // Print state every 50 steps
        // if (step % 50 == 0) {
        //     std::cout << "\nStep " << step << ":" << std::endl;
        //     print_body_state(bodies[0], "Sun");
        //     print_body_state(bodies[1], "Planet");
        //     print_body_state(bodies[2], "Photon");
        // }
    }

    std::cout << "\nFinal state:" << std::endl;
    print_body_state(bodies[0], "Sun");
    print_body_state(bodies[1], "Planet");
    print_body_state(bodies[2], "Photon");
}

int main() {
    std::cout << "Testing gravitational physics system with massive and massless bodies" << std::endl;

    test_2d_physics();
    test_3d_physics();

    std::cout << "\nSimulation complete." << std::endl;
    return 0;
}
