#include <glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "util.h"
#include "cgMath.h"

#define VERSION "0.0.0"

static const int SCREEN_WIDTH = 640;
static const int SCREEN_HEIGHT = 480;

static SDL_Window *g_window = NULL;
static SDL_GLContext g_context = NULL;

static GLuint g_program_id = 0;
static GLint g_vertex_pos_2d_location = -1;
static GLint g_timer_location = -1;
static GLint g_projection_matrix_location = -1;
static GLuint g_vao = 0;
static GLuint g_vbo = 0;
static GLuint g_ibo = 0;

static cgMat4 g_projection_matrix;

static const GLfloat g_vertex_buffer_data[] = {
   -1,-1,-10,
    1,-1,-10,
   -1, 1,-10,
    1, 1,-10,
};

static const GLint g_element_buffer_data[] = {0, 1, 2, 3};

static int initGL(void) {
    SDL_Log("OpenGL Version: %s\n", glGetString(GL_VERSION));

    g_program_id = glCreateProgram();

    GLuint vertex_shader_id = 0;
    GLuint fragment_shader_id = 0;

    if (compile_shader(GL_VERTEX_SHADER, "shader.vert", &vertex_shader_id) != 0) {
        return -1;
    }

    if (compile_shader(GL_FRAGMENT_SHADER, "shader.frag", &fragment_shader_id) != 0) {
        return -1;
    }

    glAttachShader(g_program_id, vertex_shader_id);
    glAttachShader(g_program_id, fragment_shader_id);

    if (link_program(g_program_id) != 0) {
        return -1;
    }

    get_attrib_location(g_program_id, "LVertexPos2D", &g_vertex_pos_2d_location);
    get_uniform_location(g_program_id, "timer", &g_timer_location);
    get_uniform_location(g_program_id, "projection_matrix", &g_projection_matrix_location);

    glGenVertexArrays(1, &g_vao);
    glBindVertexArray(g_vao);

    // create VBO
    glGenBuffers(1, &g_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // create IBO
    glGenBuffers(1, &g_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_element_buffer_data), g_element_buffer_data, GL_STATIC_DRAW);

    glClearColor(0.5f, 0.69, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    cgMat4_perspective(&g_projection_matrix, 65.0, SCREEN_WIDTH * 1.0 / SCREEN_HEIGHT, 0.1, 60.0);

    return 0;
}

static int init(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Failed to initialize SDL2. %s\n", SDL_GetError());
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
        SDL_Log("Failed to create SDL2 window: %s\n", SDL_GetError());
        return -1;
    }

    g_context = SDL_GL_CreateContext(g_window);
    if (g_context == NULL) {
        SDL_Log("Failed to create OpenGL context: %s\n", SDL_GetError());
        return -1;
    }

    // initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        SDL_Log("Failed to initialize GLEW. %s\n", glewGetErrorString(err));
    }

    return initGL();
}

static void handle_keys(unsigned char key, int x, int y) {
}

static void render(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(g_program_id);

    glUniform1f(g_timer_location, SDL_GetTicks());
    glUniformMatrix4fv(g_projection_matrix_location, 1, CG_MATH_GL_MATRIX_TRANSPOSE, (void *)&g_projection_matrix);

    glEnableVertexAttribArray(g_vertex_pos_2d_location);

    glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
    glVertexAttribPointer(g_vertex_pos_2d_location, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL);

    glDisableVertexAttribArray(g_vertex_pos_2d_location);
}

static void close(void) {
    glDeleteProgram(g_program_id);

    SDL_DestroyWindow(g_window);

    SDL_Quit();
}

int main(int argc, char *argv[]) {
    if (init() != 0) {
        SDL_Log("Failed to initialize program.\n");
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
