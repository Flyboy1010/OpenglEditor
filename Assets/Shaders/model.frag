#version 330 core

layout(location = 0) out vec4 outputColor;
layout(location = 1) out int outputId;

uniform int u_id;
uniform vec3 u_cameraPosition;
uniform sampler2D u_textureDiffuse;
uniform sampler2D u_textureNormal;

in vec2 v_textureUV;
in vec3 v_fragmentPosition;
in mat3 v_TBN;

void main()
{
	// light properties

	vec3 lightPosition = vec3(20, 20, 20);
	vec3 lightDirection = normalize(lightPosition - v_fragmentPosition);

	// calculate the normal using the normal map and the TBN matrix

	vec3 fragmentNormal = vec3(texture(u_textureNormal, v_textureUV)) * 2 - 1.0;
    vec3 normal = normalize(v_TBN * fragmentNormal); 

	// ambient light

	float ambientIntensity = 0.2;
	vec4 ambientColor = texture(u_textureDiffuse, v_textureUV);
	vec3 ambient = ambientIntensity * vec3(ambientColor);

	// diffuse light

	float diffuseIntensity = clamp(dot(normal, lightDirection), 0.0, 1.0);
	vec4 diffuseColor = texture(u_textureDiffuse, v_textureUV);
	vec3 diffuse = diffuseIntensity * vec3(diffuseColor);

	// specular light

	vec3 viewDirection = normalize(u_cameraPosition - v_fragmentPosition);
    float specularStrength = 0.5;
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularIntensity = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
    vec3 specular = specularStrength * specularIntensity * vec3(1.0, 1.0, 1.0);

	// if the pixel is transparent discard it

	if (diffuseColor.a <= 0.5)
		discard;

	// output color and id

	outputColor = vec4(ambient + diffuse + specular, 1.0);
	outputId = u_id;
}