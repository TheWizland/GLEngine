#version 430

layout (quads, equal_spacing, ccw) in;

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

in vec2 tcs_out[];
out vec2 tes_out;
out vec3 varyingVertexPosition;
out vec3 varyingLightDirection;
out float height;

float heightMult = 1.0;

void main(void)
{
	//Tex Coord Out
	tes_out = vec2(tcs_out[0].x + gl_TessCoord.x/patchCount, tcs_out[0].y + (1.0-gl_TessCoord.y)/patchCount);


	vec2 t0 = (tcs_out[1] - tcs_out[0]) * gl_TessCoord.x + tcs_out[0]; 
	vec2 t1 = (tcs_out[3] - tcs_out[2]) * gl_TessCoord.x + tcs_out[2];
	vec2 texCoord = (t1 - t0) * gl_TessCoord.y + t0;
	height = texture(texHeight, texCoord).r * heightMult;

	vec4 uVec = gl_in[1].gl_Position - gl_in[0].gl_Position;
    vec4 vVec = gl_in[2].gl_Position - gl_in[0].gl_Position;
    vec4 normal = normalize(vec4(cross(vVec.xyz, uVec.xyz), 0));

	vec4 p0 = (gl_in[1].gl_Position - gl_in[0].gl_Position) * gl_TessCoord.x + gl_in[0].gl_Position;
	vec4 p1 = (gl_in[3].gl_Position - gl_in[2].gl_Position) * gl_TessCoord.x + gl_in[2].gl_Position;
	vec4 point = (p1 - p0) * gl_TessCoord.y + p0;
	point += normal * height;

	gl_Position = p_matrix * v_matrix * m_matrix * point;


	varyingVertexPosition = (m_matrix * point).xyz;
	varyingLightDirection = light.position - varyingVertexPosition;
}
