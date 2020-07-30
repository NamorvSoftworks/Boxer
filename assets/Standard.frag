#version 450 core

layout(location = 0) in vec3 fPos;

layout(location = 0) out vec4 Color;

void main() {
	Color = vec4(fPos.xyz, 1.0);
}
