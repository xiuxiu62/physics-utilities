#include "graphics/mesh.hpp"
#include "graphics/shapes.hpp"
#include <cstddef>
#include <cstdlib>
#include <cstring>

struct AttributeKindMeta {
    u32 size;
    GLenum type;
};

static AttributeKindMeta get_attr_kind_meta(AttributeKind kind);

void init_mesh(Mesh &mesh) {
    mesh = {
        .vertices = nullptr,
        .vertex_count = 0,
        .indices = nullptr,
        .index_count = 0,
        .attributes = nullptr,
        .attribute_count = 0,
        .vao = 0,
        .vbo = 0,
        .ebo = 0,
        .built = false,
        .vertex_size = sizeof(Vertex),
    };

    mesh.attribute_count = Vertex::FIELD_COUNT;
    mesh.attributes = (VertexAttribute *)malloc(mesh.attribute_count * sizeof(VertexAttribute));

    // struct Vertex {
    //     Vec3 position;
    //     Vec3 normal;
    //     Vec2 uv0;
    //     Vec4 tangent;
    //     Vec4 color;

    //     static constexpr usize FIELD_COUNT = 3;
    // };

    mesh.attributes[0] = {"position", AttributeKind::VEC3, 0, offsetof(Vertex, position), false};
    mesh.attributes[1] = {"normal", AttributeKind::VEC3, 1, offsetof(Vertex, normal), false};
    mesh.attributes[2] = {"uv0", AttributeKind::VEC2, 2, offsetof(Vertex, uv0), false};
    mesh.attributes[3] = {"tangent", AttributeKind::VEC4, 3, offsetof(Vertex, tangent), false};
    mesh.attributes[4] = {"color", AttributeKind::VEC4, 4, offsetof(Vertex, color), false};
}

void deinit_mesh(Mesh &mesh) {
    // Free CPU memory
    if (mesh.vertices) {
        free(mesh.vertices);
        mesh.vertices = nullptr;
    }

    if (mesh.indices) {
        free(mesh.indices);
        mesh.indices = nullptr;
    }

    if (mesh.attributes) {
        free(mesh.attributes);
        mesh.attributes = nullptr;
    }

    // Free GPU resources
    if (mesh.vao) {
        glDeleteVertexArrays(1, &mesh.vao);
        mesh.vao = 0;
    }

    if (mesh.vbo) {
        glDeleteBuffers(1, &mesh.vbo);
        mesh.vbo = 0;
    }

    if (mesh.ebo) {
        glDeleteBuffers(1, &mesh.ebo);
        mesh.ebo = 0;
    }

    mesh.vertex_count = 0;
    mesh.index_count = 0;
    mesh.attribute_count = 0;
    mesh.built = false;
}

void set_vertices(Mesh &mesh, Vertex *vertices, usize count) {
    if (mesh.vertices && mesh.vertices != vertices) {
        free(mesh.vertices);
    }

    mesh.vertices = vertices;
    mesh.vertex_count = count;
}

void set_indices(Mesh &mesh, u32 *indices, usize count) {
    if (mesh.indices && mesh.indices != indices) {
        free(mesh.indices);
    }

    mesh.indices = indices;
    mesh.index_count = count;
}

void add_attribute(Mesh &mesh, const char *name, AttributeKind kind, int location = -1, usize offset = SIZE_MAX,
                   bool normalized = false) {
    // Duplicate the attribute name to avoid dangling pointers
    char *name_copy = strdup(name);
    if (!name_copy) {
        // error("[Mesh] Failed to copy name \"%s\"", name);
        return;
    }

    // Resize attributes array
    usize new_index = mesh.attribute_count;
    mesh.attribute_count++;
    mesh.attributes = (VertexAttribute *)realloc(mesh.attributes, mesh.attribute_count * sizeof(VertexAttribute));

    // Determine location if not specified
    if (location < 0) {
        // Use the attribute count as the location (assumes sequential binding)
        location = new_index;
    }

    // Determine offset if not specified
    if (offset == SIZE_MAX) {
        if (new_index > 0) {
            // Calculate based on previous attribute
            VertexAttribute &prev = mesh.attributes[new_index - 1];
            AttributeKindMeta prev_meta = get_attr_kind_meta(prev.kind);
            offset = prev.offset + prev_meta.size * sizeof(float);
        } else {
            // First attribute starts at offset 0
            offset = 0;
        }
    }

    // Add the new attribute
    mesh.attributes[new_index] = {name_copy, kind, (usize)location, offset, normalized};

    // Update vertex size if needed
    AttributeKindMeta meta = get_attr_kind_meta(kind);
    usize attr_end = offset + meta.size * (meta.type == GL_FLOAT ? sizeof(float) : sizeof(int));
    if (attr_end > mesh.vertex_size) {
        mesh.vertex_size = attr_end;
    }

    // Mark mesh as needing to be rebuilt
    mesh.built = false;
}

// void add_attribute(Mesh &mesh, const char *name, AttributeKind kind, usize location, usize offset, bool normalized) {
//     mesh.attribute_count++;
//     mesh.attributes = (VertexAttribute *)realloc(mesh.attributes, mesh.attribute_count * sizeof(VertexAttribute));
//     mesh.attributes[mesh.attribute_count - 1] = attr;
// }

void build_mesh(Mesh &mesh) {
    if (mesh.vertex_count == 0 || !mesh.vertices) {
        // error("[Mesh] No vertices to build");
        return;
    }

    // Generate OpenGL objects if needed
    if (mesh.vao == 0) {
        glGenVertexArrays(1, &mesh.vao);
        glGenBuffers(1, &mesh.vbo);

        if (mesh.indices && mesh.index_count > 0) {
            glGenBuffers(1, &mesh.ebo);
        }
    }

    glBindVertexArray(mesh.vao);

    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertex_count * sizeof(Vertex), mesh.vertices, GL_STATIC_DRAW);

    // Upload index data if present
    if (mesh.indices && mesh.index_count > 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.index_count * sizeof(u32), mesh.indices, GL_STATIC_DRAW);
    }

    // Configure vertex attributes
    for (usize i = 0; i < mesh.attribute_count; i++) {
        VertexAttribute &attr = mesh.attributes[i];

        // Determine attribute properties based on kind
        GLboolean normalized = attr.normalized ? GL_TRUE : GL_FALSE;

        auto meta = get_attr_kind_meta(attr.kind);
        u32 size = meta.size;
        GLenum type = meta.type;

        glEnableVertexAttribArray(attr.location);

        if (type == GL_FLOAT) {
            glVertexAttribPointer(attr.location, size, type, normalized, sizeof(Vertex), (void *)attr.offset);
        } else {
            glVertexAttribIPointer(attr.location, size, type, sizeof(Vertex), (void *)attr.offset);
        }
    }

    glBindVertexArray(0);
    mesh.built = true;
}

// Draw the mesh
void draw_mesh(const Mesh &mesh) {
    if (!mesh.built || mesh.vertex_count == 0) {
        // warn("[Mesh] Trying to draw an unbuilt or empty mesh");
        return;
    }

    glBindVertexArray(mesh.vao);

    if (mesh.indices && mesh.index_count > 0) {
        glDrawElements(GL_TRIANGLES, mesh.index_count, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, mesh.vertex_count);
    }

    glBindVertexArray(0);
}

void update_mesh_vertices(Mesh &mesh, Vertex *vertices, usize count) {
    if (count == 0 || !vertices) return;

    // Update local copy
    set_vertices(mesh, vertices, count);

    // Update GPU buffer if already built
    if (mesh.built) {
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glBufferData(GL_ARRAY_BUFFER, mesh.vertex_count * sizeof(Vertex), mesh.vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

// Mesh generate_mesh_from_shape(Triangle shape) {
//     Mesh mesh{0};
//     memcpy(mesh.vertices, shape.vertices, 3 * sizeof(Vec2));
//     mesh.vertices[0].position mesh.vertex_count = 3;

//     static constexpr u32 indices[3]{0, 1, 2};
//     memcpy(mesh.indices, indices, 3 * sizeof(u32));
//     mesh.index_count = 3;
// }

static AttributeKindMeta get_attr_kind_meta(AttributeKind kind) {
    AttributeKindMeta meta;

    switch (kind) {
    case AttributeKind::FLOAT:
        meta.size = 1;
        meta.type = GL_FLOAT;
        break;
    case AttributeKind::VEC2:
        meta.size = 2;
        meta.type = GL_FLOAT;
        break;
    case AttributeKind::VEC3:
        meta.size = 3;
        meta.type = GL_FLOAT;
        break;
    case AttributeKind::VEC4:
        meta.size = 4;
        meta.type = GL_FLOAT;
        break;
    case AttributeKind::INT:
        meta.size = 1;
        meta.type = GL_INT;
        break;
    case AttributeKind::IVEC2:
        meta.size = 2;
        meta.type = GL_INT;
        break;
    case AttributeKind::IVEC3:
        meta.size = 3;
        meta.type = GL_INT;
        break;
    case AttributeKind::IVEC4:
        meta.size = 4;
        meta.type = GL_INT;
        break;
    case AttributeKind::UINT:
        meta.size = 1;
        meta.type = GL_UNSIGNED_INT;
        break;
    case AttributeKind::BOOL:
        meta.size = 1;
        meta.type = GL_BOOL;
        break;
    }

    return meta;
}
