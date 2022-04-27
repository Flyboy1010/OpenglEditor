#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureUV;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

uniform mat4 u_projection;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_lightSpace;

out vec2 v_textureUV;
out vec3 v_fragmentPosition;
out vec4 v_fragmentPositionLightSpace;
out mat3 v_TBN;

void main()
{
	// calculate the tbn matrix

	vec3 T = normalize(vec3(u_model * vec4(tangent,   0.0)));
    vec3 B = normalize(vec3(u_model * vec4(bitangent, 0.0)));
    vec3 N = normalize(vec3(u_model * vec4(normal,    0.0)));
    v_TBN = mat3(T, B, N);

	// pass textureUV

	v_textureUV = textureUV;

	// pass fragment position

	vec4 vertexPositionWorldSpace = u_model * vec4(position, 1.0);
	v_fragmentPosition = vec3(vertexPositionWorldSpace);

	// pass the fragment position in light space

	v_fragmentPositionLightSpace = u_lightSpace * vec4(v_fragmentPosition, 1.0);

	// position

	gl_Position = u_projection * u_view * vertexPositionWorldSpace;
}