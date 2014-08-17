#version 330 core

uniform float timer;
uniform mat4 projection_matrix;
in vec4 LVertexPos2D;

flat out int instance;

mat4 translate(vec3 t) {
    return mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        t.x, t.y, t.z, 1.0
    );
}

mat4 rotate(vec3 axis, float angle) {
    vec3 a = normalize(axis);
    float x = a.x;
    float y = a.y;
    float z = a.z;
    float s = sin(angle);
    float c = cos(angle);
    float m = 1.0 - c;
    return mat4(
        m * x * x + c, m * x * y - z * s, m * z * x + y * s, 0.0,
        m * x * y + z * s, m * y * y + c, m * y * z - x * s, 0.0,
        m * z * x - y * s, m * y * z + x * s, m * z * z + c, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

void main() {
    float t = timer;
    vec4 p = LVertexPos2D;
    int i = gl_InstanceID;
    instance = i;
    int x = i % 9 - 4;
    int y = i / 9 - 4;
    p = rotate(vec3(0, 1, 0), t * 4) * p;
    p = translate(vec3(sin(t) * 4 + x * 2, y * 2, cos(t) * 10 - 20)) * p;
    p = projection_matrix * p;
    gl_Position = p;
}
