#include <glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "util.h"

#define VERSION "0.0.0"

static const int SCREEN_WIDTH = 640;
static const int SCREEN_HEIGHT = 480;

static SDL_Window *g_window = NULL;
static SDL_GLContext g_context = NULL;

static GLuint g_program_id = 0;
static GLint g_vertex_pos_2d_location = -1;
static GLuint g_vao = 0;
static GLuint g_vbo = 0;
static GLuint g_ibo = 0;

static int g_render_quad = 1;

static const char *g_vertex_shader = "                                        \
    #version 330 core                                                       \n\
    in vec2 LVertexPos2D;                                                   \n\
    void main() {                                                           \n\
        gl_Position = vec4(LVertexPos2D.x, LVertexPos2D.y, 0, 1);           \n\
    }                                                                       \n\
";

static const char *g_fragment_shader = "                                    \n\
    #version 330 core                                                       \n\
    out vec4 LFragment;                                                     \n\
    void main() {                                                           \n\
        LFragment = vec4(1.0, 1.0, 1.0, 1.0);                               \n\
    }                                                                       \n\
";

static int initGL(void) {
    g_program_id = glCreateProgram();

    GLuint vertex_shader_id = 0;
    GLuint fragment_shader_id = 0;

    if (compile_shader_from_memory(GL_VERTEX_SHADER, g_vertex_shader, strlen(g_vertex_shader), &vertex_shader_id) != 0) {
        return -1;
    }

    if (compile_shader_from_memory(GL_FRAGMENT_SHADER, g_fragment_shader, strlen(g_fragment_shader), &fragment_shader_id) != 0) {
        return -1;
    }

    glAttachShader(g_program_id, vertex_shader_id);
    glAttachShader(g_program_id, fragment_shader_id);

    if (link_program(g_program_id) != 0) {
        return -1;
    }

    g_vertex_pos_2d_location = glGetAttribLocation(g_program_id, "LVertexPos2D");
    if (g_vertex_pos_2d_location == -1) {
        printf("\"LVertexPos2D\" is not a valid GLSL program variable.\n");
        return -1;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    GLfloat vertex_data[] = {
       -1.0f,-1.0f, 0.0f,
        1.0f,-1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };

    GLuint index_data[] = {0, 1, 2, 3};

    glGenVertexArrays(1, &g_vao);
    glBindVertexArray(g_vao);

    // create VBO
    glGenBuffers(1, &g_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    // create IBO
    /*
    glGenBuffers(1, &g_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), index_data, GL_STATIC_DRAW);
    */

    return 0;
}

static int init(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("Failed to initialize SDL2. %s\n", SDL_GetError());
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    g_window = SDL_CreateWindow("TerraCraft - "VERSION,
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                SCREEN_WIDTH, SCREEN_HEIGHT,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (g_window == NULL) {
        printf("Failed to create SDL2 window. %s\n", SDL_GetError());
        return -1;
    }

    g_context = SDL_GL_CreateContext(g_window);
    if (g_context == NULL) {
        printf("Failed to create OpenGL context. %s\n", SDL_GetError());
        return -1;
    }

    // initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        printf("Failed to initialize GLEW. %s\n", glewGetErrorString(err));
    }

    return initGL();
}

static void handle_keys(unsigned char key, int x, int y) {
    if (key == 'q') {
        g_render_quad = !g_render_quad;
    }
}

static void render(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    if (g_render_quad) {
        /*glUseProgram(g_program_id);*/

        glEnableVertexAttribArray(g_vertex_pos_2d_location);

        glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
        glVertexAttribPointer(g_vertex_pos_2d_location, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo);*/
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glDisableVertexAttribArray(g_vertex_pos_2d_location);
    }
}

static void close(void) {
    glDeleteProgram(g_program_id);

    SDL_DestroyWindow(g_window);

    SDL_Quit();
}

int main(void) {
    if (init() != 0) {
        printf("Failed to initialize program.\n");
        return -1;
    }

    int is_quit = 0;
    SDL_Event e;

    SDL_StartTextInput();

    while (!is_quit) {
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT:
                    is_quit = 1;
                    break;
                case SDL_TEXTINPUT: {
                    int x = 0, y = 0;
                    SDL_GetMouseState(&x, &y);
                    handle_keys(e.text.text[0], x, y);
                    break;
                }
            }
        }

        render();

        SDL_GL_SwapWindow(g_window);
    }

    SDL_StopTextInput();

    close();

    return 0;
}
