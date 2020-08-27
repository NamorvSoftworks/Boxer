#version 450 core

layout(location = 0) in vec3 v_Pos;

layout(location = 0) out vec4 o_Color;

void main() {
	o_Color = vec4(v_Pos.xyz, 1.0);
}
