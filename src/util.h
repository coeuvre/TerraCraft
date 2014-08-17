#ifndef UTIL_H
#define UTIL_H

#include <SDL_opengl.h>

#define GLSL(x) #x

int compile_shader(GLenum type, const char *path, GLuint *shader_id);

int compile_shader_from_memory(GLenum type, const GLchar *buf, GLint len,
                               GLuint *shader_id);

int link_program(GLuint program_id);

int get_attrib_location(GLuint program_id, const char *varname, GLint *location);

int get_uniform_location(GLuint program_id, const char *varname, GLint *location);

void make_cube(float x, float y, float z, float n, float *output);

char *resource_path(const char *base, char *filepath, char *buf, int len);

#endif // UTIL_H
