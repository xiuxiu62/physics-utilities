#include "common/logger.hpp"
#include "common/types.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

static const char *TITLE = "example";
static const u32 WIDTH = 1920, HEIGHT = 1080;
static const bool VSYNC_ENABLED = true;

void handle_input(GLFWwindow *windo);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void APIENTRY gl_debug_message(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
                               const char *message, const void *userParam);

struct Engine {
    GLFWwindow *window;
};

void init_window(GLFWwindow *&window, const char *title, u32 width, u32 height, bool enable_vsync) {
    if (!glfwInit()) {
        error("Failed to initialize glfw");
        window = nullptr;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    when_debug(glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE));

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        error("Failed to create window");
        return;
    }

    glfwMakeContextCurrent(window);
    if (enable_vsync) glfwSwapInterval(1); // enable vsync
}

i32 init_engine(Engine &engine, const char *title, u32 width, u32 height, bool enable_vsync) {
    init_window(engine.window, title, width, height, enable_vsync);
    if (!engine.window) {
        return -1;
    }

    glfwSetFramebufferSizeCallback(engine.window, framebuffer_size_callback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        error("Failed to initialize GLAD");
        glfwTerminate();
        return -1;
    }

    when_debug({
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(gl_debug_message, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    });

    // TODO: Ensure this works for 3D
    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(0, 0, 0, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return 0;
}

void deinit_engine(Engine &engine) {
    glfwTerminate();
}

void run_engine(Engine &engine) {
    while (!glfwWindowShouldClose(engine.window)) {
        // Pre-Update
        glfwPollEvents();

        // Update
        handle_input(engine.window);

        // Post-Update

        // Pre-Render
        glClear(GL_COLOR_BUFFER_BIT);

        // Render

        // Post-Render
        glfwSwapBuffers(engine.window);
    }
}

int main() {
    Engine engine;

    if (init_engine(engine, TITLE, WIDTH, HEIGHT, VSYNC_ENABLED)) {
        error("Failed to initialize engine, exiting");
        return -1;
    };

    run_engine(engine);
    deinit_engine(engine);

    return 0;
}

void handle_input(GLFWwindow *window) {
#define on_press(key, ...)                                                                                             \
    if (glfwGetKey(window, key) == GLFW_PRESS) __VA_ARGS__;
#define on_release(key, ...)                                                                                           \
    if (glfwGetKey(window, key) == GLFW_RELEASE) __VA_ARGS__;

    on_press(GLFW_KEY_ESCAPE, glfwSetWindowShouldClose(window, true));
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void APIENTRY gl_debug_message(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
                               const char *message, const void *userParam) {
    const char *sourceString;
    switch (source) {
    case GL_DEBUG_SOURCE_API:
        sourceString = "API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        sourceString = "Window System";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        sourceString = "Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        sourceString = "Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        sourceString = "Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        sourceString = "Other";
        break;
    default:
        sourceString = "Unknown";
    }
    const char *typeString;
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        typeString = "Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        typeString = "Deprecated Behavior";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        typeString = "Undefined Behavior";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        typeString = "Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        typeString = "Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        typeString = "Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        typeString = "Push Group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        typeString = "Pop Group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        typeString = "Other";
        break;
    default:
        typeString = "Unknown";
    }
    printf("[GL %s:%s] %s (ID: %u, Severity: %d)\n\n", sourceString, typeString, message, id, severity);
}
