#version 430

in vec2 textureCoordinate;
in vec3 varyingNormal;
in vec3 varyingVertexPosition;
layout (binding = 0) uniform sampler2D samp;
layout (binding = 1) uniform sampler2D height;
out vec4 color;

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
uniform vec4 globalAmbient;

vec3 L, N, V, R;
vec4 ambient, diffuse, specular;
vec3 halfVector;
float cosTheta, cosPhi;
float dist, attenuationFactor, intensity;
vec4 texColor;

void main(void) {
	L = normalize(light.position - varyingVertexPosition);
	N = normalize(varyingNormal);
	V = normalize(-v_matrix[3].xyz - varyingVertexPosition);

	cosTheta = dot(L,N);
	halfVector = normalize(L + V);
    cosPhi = dot(halfVector,N);

	dist = distance(varyingVertexPosition, light.position);
	attenuationFactor = 1.0 / (light.constantAttenuation + light.linearAttenuation*dist + light.quadraticAttenuation*dist*dist);

	ambient = (globalAmbient + light.ambient * material.ambient);
	diffuse = (light.diffuse * material.diffuse * max(cosTheta,0.0))*attenuationFactor;
	specular = (light.specular * material.specular * pow(max(cosPhi,0.0), material.shininess))*attenuationFactor;

    texColor = texture(samp, textureCoordinate);
	color = texColor * (ambient + diffuse + specular);
	//Alternatively:
    //color = texColor * (ambient + diffuse) + specular;
}