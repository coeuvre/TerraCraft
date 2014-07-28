#version 330 core

uniform float timer;
uniform mat4 projection_matrix;
in vec3 LVertexPos2D;

void main() {
    gl_Position = projection_matrix * vec4(LVertexPos2D.xyz, 1);
}
