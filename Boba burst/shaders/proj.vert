#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;

layout(location = 0) out vec4 v_color;

layout(std140, push_constant) uniform Matrices
{
	mat4 worldViewProjMatrix;
};

void main()
{
	v_color = a_color;
	gl_Position = worldViewProjMatrix * vec4(a_position, 1);
}
