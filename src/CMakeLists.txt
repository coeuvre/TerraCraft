include(FindOpenGL)
include(FindPkgConfig)

pkg_check_modules(SDL2 REQUIRED sdl2)
separate_with_space(SDL2_CFLAGS)
separate_with_space(SDL2_LDFLAGS)

pkg_check_modules(GLEW REQUIRED glew)
separate_with_space(GLEW_CFLAGS)
separate_with_space(GLEW_LDFLAGS)

file(GLOB TerraCraft_SOURCE_FILES "*.c")

add_executable(TerraCraft ${TerraCraft_SOURCE_FILES})
set_target_properties(
    TerraCraft
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)

message(${SDL2_CFLAGS} ${SDL2_LDCFLAGS} ${GLEW_CFLAGS} ${GLEW_LDFLAGS})
set(CMAKE_C_FLAGS "${SDL2_CFLAGS} ${GLEW_CFLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${SDL2_LDFLAGS} ${GLEW_LDFLAGS}")

target_link_libraries(
    TerraCraft
    ${OPENGL_gl_LIBRARY}
)
