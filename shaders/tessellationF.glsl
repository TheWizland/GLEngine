#version 430

layout (binding = 0) uniform sampler2D texColor;
layout (binding = 1) uniform sampler2D texHeight;
layout (binding = 2) uniform sampler2D texNormal;
layout (binding = 3) uniform sampler2DShadow texShadow;

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

in float height;
in vec2 tes_out;
in vec3 varyingVertexPosition;
in vec3 varyingLightDirection;
in vec4 shadow_coord;
out vec4 color;

vec4 sampColor;
vec4 ambient, diffuse, specular;

float lookup(float x, float y) {
    float texVal = textureProj(texShadow, shadow_coord + vec4(x * 0.001 * shadow_coord.w,
                                                         y * 0.001 * shadow_coord.w,
                                                         -0.01, 0.0));
    return texVal;
}

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

	float shadowFactor = 0;
	float swidth = 2.5;
    vec2 o = mod(floor(gl_FragCoord.xy), 2.0) * swidth;
    shadowFactor += lookup(-1.5*swidth + o.x,  1.5*swidth - o.y);
    shadowFactor += lookup(-1.5*swidth + o.x, -0.5*swidth - o.y);
    shadowFactor += lookup( 0.5*swidth + o.x,  1.5*swidth - o.y);
    shadowFactor += lookup( 0.5*swidth + o.x, -0.5*swidth - o.y);
    shadowFactor = shadowFactor / 4.0;

	sampColor = texture(texColor, tes_out);
	//color = sampColor * (ambient + diffuse + specular);
	color = sampColor * (ambient + shadowFactor*(diffuse + specular));
}
