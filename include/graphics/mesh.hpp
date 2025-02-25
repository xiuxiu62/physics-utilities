#pragma once

#include "math/vec2.hpp"
#include "math/vec3.hpp"
#include "math/vec4.hpp"
#include <glad/glad.h>

enum class AttributeKind {
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    INT,
    IVEC2,
    IVEC3,
    IVEC4,
    UINT,
    BOOL,
};

struct VertexAttribute {
    const char *name;
    AttributeKind kind;
    usize location;
    usize offset;
    bool normalized;
};

struct Vertex {
    Vec3 position;
    Vec3 normal;
    Vec2 uv0;
    Vec4 tangent;
    Vec4 color;

    static constexpr usize FIELD_COUNT = 5;
};

struct Mesh {
    Vertex *vertices;
    usize vertex_count;

    u32 *indices;
    usize index_count;

    VertexAttribute *attributes;
    usize attribute_count;

    u32 vao, vbo, ebo;

    bool built;
    usize vertex_size; // Size of vertex structure in bytes
};

void init_mesh(Mesh &mesh);
void deinit_mesh(Mesh &mesh);

void set_vertices(Mesh &mesh, Vertex *vertices, usize count);
void set_indices(Mesh &mesh, u32 *indices, usize count);

void add_attribute(Mesh &mesh, const char *name, VertexAttribute attr);
void build_mesh(Mesh &mesh);
void draw_mesh(const Mesh &mesh);
void update_mesh_vertices(Mesh &mesh, Vertex *vertices, usize count);
