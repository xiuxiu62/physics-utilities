struct Shader {
    GLuint id;
    // const char *vertex_source;
    // const char *fragment_source;
    // const char *geometry_source;
    // struct {
    //     const char *control;
    //     const char *evaluation;
    // } tesselation_source;
};

void init_shader(Shader &shader, const char *vertex_source, const char *fragment_source, const char *geometry_source,
                 const char *tesselation_control_source, const char *tesselation_evaluation_source) {
    // TODO: unimplemented
}

void deinit_shader(Shader &shader) {
    // TODO: unimplemented
}

void set_bool(Shader &shader, const char *name, bool value) {
    // glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
