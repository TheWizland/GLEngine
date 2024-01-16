#version 430

in vec2 textureCoordinate;
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

out vec4 color;

void main(void) {
	vec3 ambient = (globalAmbient*material.ambient).xyz;
    vec4 texColor = texture(samp, textureCoordinate);
    color = texColor * vec4(ambient,1.0);
    //color = vec4(1,0,0,0);
}