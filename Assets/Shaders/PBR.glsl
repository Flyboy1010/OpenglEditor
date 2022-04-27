#type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureUV;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

uniform mat4 u_projection;
uniform mat4 u_model;
uniform mat4 u_view;

out vec2 v_textureUV;
out vec3 v_fragmentPosition;
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

	// position

	gl_Position = u_projection * u_view * vertexPositionWorldSpace;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 outputColor;
layout(location = 1) out int outputId;

uniform sampler2D u_albedoMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_metallicRoughnessMap;
uniform vec3 u_cameraPosition;
uniform int u_id;

in vec2 v_textureUV;
in vec3 v_fragmentPosition;
in mat3 v_TBN;

const float PI = 3.14159265359;
  
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 GetNormalFromNormalMap(sampler2D normalMap, vec2 uv)
{
    vec3 fragmentNormal = vec3(texture(normalMap, uv)) * 2 - 1.0;
    vec3 normal = normalize(v_TBN * fragmentNormal);
    return normal;
}

void main()
{
    // info from maps

    vec3 albedo     = pow(texture(u_albedoMap, v_textureUV).rgb, vec3(2.2));
    vec3 normal     = GetNormalFromNormalMap(u_normalMap, v_textureUV);
    float metallic  = texture(u_metallicRoughnessMap, v_textureUV).b;
    float roughness = texture(u_metallicRoughnessMap, v_textureUV).g;
    float ao        = 1.0;

    if (texture(u_albedoMap, v_textureUV).a <= 0.5)
        discard;

    // light properties

    vec3 lightDirection = normalize(vec3(0.0, 0.0, 0.0) - vec3(15.0, 30.0, 15.0));
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    // PBR

    vec3 N = normalize(normal);
    vec3 V = normalize(u_cameraPosition - v_fragmentPosition);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);
	           
    // reflectance equation

    vec3 Lo = vec3(0.0);

    // calculate per-light radiance

    vec3 L = -lightDirection;
    vec3 H = normalize(V + L);
    // float distance    = length(lightPositions[i] - v_fragmentPosition);
    // float attenuation = 1.0 / (distance * distance);
    // vec3 radiance     = lightColors[i] * attenuation;
    vec3 radiance     = lightColor;
        
    // cook-torrance brdf

    float NDF = DistributionGGX(N, H, roughness);        
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
        
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;	  
        
    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular     = numerator / denominator;  
            
    // add to outgoing radiance Lo

    float NdotL = max(dot(N, L), 0.0);                
    Lo += (kD * albedo / PI + specular) * radiance * NdotL;
  
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;
	
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  
   
    // output

    outputColor = vec4(color, 1.0);
    outputId = u_id;
}