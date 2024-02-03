#version 430

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 vertexNormal;
layout (binding = 0) uniform sampler2D samp;
out vec2 textureCoordinate;
out vec3 varyingNormal;
out vec3 varyingVertexPosition;

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
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;
uniform vec4 globalAmbient;
uniform int internallyLit;

void main(void) {
	varyingVertexPosition = (m_matrix * vec4(vertexPosition, 1.0)).xyz;
    varyingNormal = (norm_matrix * vec4(vertexNormal, 1.0)).xyz;
    //varyingNormal = vec3(internallyLit*varyingNormal.x, internallyLit*varyingNormal.y, internallyLit*varyingNormal.z);
	varyingNormal = internallyLit * varyingNormal;

	gl_Position = proj_matrix * v_matrix * m_matrix * vec4(vertexPosition, 1);
	textureCoordinate = texCoord;
}