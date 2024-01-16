#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (binding = 0) uniform sampler2D samp;

struct Light {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 position;
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
uniform vec4 globalAmbient;

out vec2 textureCoordinate;

void main(void) {
	gl_Position = proj_matrix * v_matrix * m_matrix * vec4(position, 1);
	textureCoordinate = texCoord;
}