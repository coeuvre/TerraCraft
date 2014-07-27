include(FindOpenGL)
include(FindPkgConfig)

pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(GLEW REQUIRED glew)

file(GLOB TerraCraft_SOURCE_FILES "*.c")

add_executable(TerraCraft ${TerraCraft_SOURCE_FILES})
set_target_properties(
    TerraCraft
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)

set(CMAKE_C_FLAGS "-D_THREAD_SAFE")

include_directories(
    ${GLEW_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(
    TerraCraft
    ${OPENGL_gl_LIBRARY}
    ${GLEW_LIBRARIES}
    ${SDL2_LIBRARIES}
)
