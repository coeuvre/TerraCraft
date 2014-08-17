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

mat4 rotationMatrix(vec3 axis, float angle) {
    vec3 a = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    return mat4(
        oc * a.x * a.x + c,
        oc * a.x * a.y - a.z * s,
        oc * a.z * a.x + a.y * s,
        0.0,
        oc * a.x * a.y + a.z * s,
        oc * a.y * a.y + c,
        oc * a.y * a.z - a.x * s,
        0.0,
        oc * a.z * a.x - a.y * s,
        oc * a.y * a.z + a.x * s,
        oc * a.z * a.z + c,
        0.0,
        0.0,
        0.0,
        0.0,
        1.0
    );
}

void main() {
    vec4 p = LVertexPos2D;
    int i = gl_InstanceID;
    instance = i;
    int x = i % 9 - 4;
    int y = i / 9 - 4;
    p = translate(vec3(sin(timer * 0.003) * 4 + x * 2, y * 2, -20)) * p;
    p = projection_matrix * p;
    gl_Position = p;
}
