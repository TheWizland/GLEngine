#version 430

in vec2 textureCoordinate;
in vec3 varyingNormal;
in vec3 varyingVertexPosition;
layout (binding = 0) uniform sampler2D samp;
out vec4 color;

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

vec3 lightDirection, L, N, V, H, R;
vec4 ambient, diffuse, specular;
vec3 halfVector;
float cosTheta, cosPhi;
float dist, attenuationFactor, intensity;
vec4 texColor;

void main(void) {
	lightDirection = light.position - varyingVertexPosition;
	L = normalize(lightDirection);
	N = normalize(varyingNormal);
	V = normalize(-v_matrix[3].xyz - varyingVertexPosition);

	cosTheta = dot(L,N);

	//This method creates specular highlights that do not move with the camera angle.
	//halfVector = normalize(L + normalize(-varyingVertexPosition)).xyz;
    //H = normalize(halfVector);
    //cosPhi = dot(H,N);

	//Specular highlights do follow camera angle.
	//However, this produces highlights on the opposite side of the object if cosTheta is not checked.
	//Also results in some graphical oddities.
	R = normalize(reflect(-L, N));
	cosPhi = cosTheta > 0 ? dot(V,R) : 0.0;

	ambient = (globalAmbient + light.ambient * material.ambient);
	diffuse = (light.diffuse * material.diffuse * max(cosTheta,0.0));
	specular = (light.specular * material.specular * pow(max(cosPhi,0.0), material.shininess*3.0));

    texColor = texture(samp, textureCoordinate);
    color = texColor * (ambient + diffuse) + specular;
}