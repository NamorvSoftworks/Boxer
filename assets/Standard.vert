#version 450 core

layout(location = 0) in vec3 a_Pos;

layout(location = 0) out vec3 v_Pos;

void main() {
    v_Pos = a_Pos;
    gl_Position = vec4(v_Pos, 1.0);
}
