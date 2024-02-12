#version 430

layout (binding = 0) uniform sampler2D texColor;
layout (binding = 1) uniform sampler2D texHeight;
layout (binding = 2) uniform sampler2D texNormal;

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
uniform vec4 globalAmbient;
uniform int patchCount;

in float height;
in vec2 tes_out;
in vec3 varyingVertexPosition;
in vec3 varyingLightDirection;
out vec4 color;

vec4 sampColor;
vec4 ambient, diffuse, specular;

vec3 calcNewNormal() {
	//Assumed that patch is facing up and not rotated.
	//TODO: Modify so that this assumption is not made?
	//Maybe multiply normal and tangent by m_matrix?
    vec3 normal = vec3(0, 1, 0);
    vec3 tangent = vec3(1, 0, 0);
    //tangent = normalize(tangent - dot(tangent, normal) * normal);
    vec3 bitangent = cross(tangent, normal);
    mat3 tbn = mat3(tangent, bitangent, normal);
    vec3 retrievedNormal = texture(texNormal, tes_out).xyz;
    retrievedNormal = retrievedNormal * 2.0 - 1.0;
    vec3 newNormal = tbn * retrievedNormal;
    newNormal = normalize(newNormal);
    return newNormal;
}

void main()
{
    vec3 N = calcNewNormal();
	vec3 L = normalize(light.position - varyingVertexPosition);
	vec3 V = normalize(-v_matrix[3].xyz - varyingVertexPosition);
	vec3 R = normalize(reflect(-L, N));
	float cosTheta = dot(L,N);
	float cosPhi = dot(V,R);

	float dist = distance(varyingVertexPosition, light.position);
	float attenuationFactor = 1.0 / (light.constantAttenuation + light.linearAttenuation*dist + light.quadraticAttenuation*dist*dist);

	ambient = (globalAmbient + light.ambient * material.ambient);
	diffuse = (light.diffuse * material.diffuse * max(cosTheta,0.0))*attenuationFactor;
	specular = (light.specular * material.specular * pow(max(cosPhi,0.0), material.shininess))*attenuationFactor;

	sampColor = texture(texColor, tes_out);
	color = sampColor * (ambient + diffuse + specular);
}
