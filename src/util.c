#include <glew.h>

#include "util.h"

#include <stdio.h>

#include <SDL.h>

static long file_length(FILE *f) {
    long pos = ftell(f);
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, pos, SEEK_SET);
    return len;
}

int compile_shader(GLenum type, const char *path, GLuint *shader_id) {
    FILE *f = fopen(path, "rb+");

    if (f == NULL) {
        SDL_Log("Failed to open file \"%s\"", path);
        return -1;
    }

    GLint len = file_length(f);
    GLchar *buf = malloc(len);
    fread(buf, 1, len, f);
    fclose(f);
    return compile_shader_from_memory(type, buf, len, shader_id);
}

int compile_shader_from_memory(GLenum type, const GLchar *buf, GLint len,
                               GLuint *shader_id) {
    GLuint id = glCreateShader(type);

    glShaderSource(id, 1, &buf, &len);

    glCompileShader(id);

    GLint status = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        SDL_Log("Failed to compile shader.\n");
#ifndef NDEBUG
        GLint log_length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);
        if (log_length > 0) {
            GLchar *log = malloc(log_length);
            glGetShaderInfoLog(id, log_length, &log_length, log);
            SDL_Log("Shader compile log: %s\n", log);
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
        SDL_Log("Failed to link program %d", program_id);
#ifndef NDEBUG
        GLint log_length = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);
        if (log_length > 0) {
            GLchar *log = malloc(log_length);
            glGetProgramInfoLog(program_id, log_length, &log_length, log);
            SDL_Log("Program link log: %s\n", log);
            free(log);
        }
#endif

        return -1;
    }

    return 0;
}

int get_attrib_location(GLuint program_id, const char *varname, GLint *location) {
    *location = glGetAttribLocation(program_id, varname);
    if (*location == -1) {
        SDL_Log("%s is not a valid GLSL program variable.\n", varname);
        return -1;
    }
    return 0;
}

int get_uniform_location(GLuint program_id, const char *varname, GLint *location) {
    *location = glGetUniformLocation(program_id, varname);
    if (*location == -1) {
        SDL_Log("%s is not a valid GLSL program uniform.\n", varname);
        return -1;
    }
    return 0;
}

void make_cube(float x, float y, float z, float n, float *output) {
    *(output++) = x - n;
    *(output++) = y - n;
    *(output++) = z - n;
    *(output++) = x - n;
    *(output++) = y - n;
    *(output++) = z + n;
    *(output++) = x - n;
    *(output++) = y + n;
    *(output++) = z - n;
    *(output++) = x - n;
    *(output++) = y + n;
    *(output++) = z + n;
    *(output++) = x + n;
    *(output++) = y - n;
    *(output++) = z - n;
    *(output++) = x + n;
    *(output++) = y - n;
    *(output++) = z + n;
    *(output++) = x + n;
    *(output++) = y + n;
    *(output++) = z - n;
    *(output++) = x + n;
    *(output++) = y + n;
    *(output++) = z + n;
}

char *resource_path(const char *base, char *filepath, char *buf, int len) {
    *buf = '\0';
    strncat(buf, base, len);
    strncat(buf, filepath, len);
    return buf;
}

