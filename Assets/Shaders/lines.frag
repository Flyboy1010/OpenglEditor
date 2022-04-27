#version 330 core

layout(location = 0) out vec4 output_color;

in vec4 v_color;

void main()
{
	output_color = v_color;
}