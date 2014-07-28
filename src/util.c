#include "util.h"

#include <stdio.h>
#include <stdlib.h>

int compile_shader(GLenum type, const char *path, GLuint *shader_id) {
    return 0;
}

int compile_shader_from_memory(GLenum type, const GLchar *buf, GLint len,
                               GLuint *shader_id) {
    GLuint id = glCreateShader(type);

    glShaderSource(id, 1, &buf, &len);

    glCompileShader(id);

    GLint status = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        printf("Failed to compile shader.\n");
#ifndef NDEBUG
        GLint log_length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);
        if (log_length > 0) {
            GLchar *log = malloc(log_length);
            glGetShaderInfoLog(id, log_length, &log_length, log);
            printf("Shader compile log: %s\n", log);
            free(log);
        }
#endif
        return -1;
    }

    *shader_id = id;
    return 0;
}

int link_program(GLuint program_id) {
    glLinkProgram(program_id);

    GLint status = 0;
    glGetProgramiv(program_id, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        printf("Failed to link program %d", program_id);
#ifndef NDEBUG
        GLint log_length = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);
        if (log_length > 0) {
            GLchar *log = malloc(log_length);
            glGetProgramInfoLog(program_id, log_length, &log_length, log);
            printf("Program link log: %s\n", log);
            free(log);
        }
#endif

        return -1;
    }

    return 0;
}
