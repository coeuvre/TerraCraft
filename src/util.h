#ifndef UTIL_H
#define UTIL_H

#include <SDL2/SDL_opengl.h>

#define GLSL(x) #x

int compile_shader(GLenum type, const char *path, GLuint *shader_id);

int compile_shader_from_memory(GLenum type, const GLchar *buf, GLint len,
                               GLuint *shader_id);

int link_program(GLuint program_id);

#endif // UTIL_H
