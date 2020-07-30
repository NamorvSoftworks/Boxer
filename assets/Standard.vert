#version 450 core

layout(location = 0) in vec3 vPos;

layout(location = 0) out vec3 fPos;

void main() {
    fPos = vPos;
    gl_Position = vec4(vPos, 1.0);
}
