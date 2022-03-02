#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;

uniform mat4 u_projection;

out vec4 v_color;

void main()
{
	v_color = color;

	gl_Position = u_projection * vec4(position, 0.0, 1.0);
}