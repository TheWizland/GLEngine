#version 430

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 texCoord;

layout (binding = 0) uniform sampler2D texColor;
layout (binding = 1) uniform sampler2D texHeight;
layout (binding = 2) uniform sampler2D texNormal;
layout (binding = 3) uniform sampler2DShadow texShadow;

out vec2 textureCoord;

struct Light {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 position;
	float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};
struct Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform Light light;
uniform Material material;
uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 p_matrix;
uniform mat4 norm_matrix;
uniform mat4 vp_shadow;
uniform vec4 globalAmbient;
uniform int patchCount;

void main(void) 
{
	gl_Position = vec4(vertexPosition, 1.0);
	textureCoord = texCoord;
}
