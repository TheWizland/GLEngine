#version 430

layout (vertices = 4) out;

in vec2 textureCoord[];
out vec2 tcs_out[];

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

void main(void)
{
	int tessLevel = 32;

	//Invocation 0 controls tess levels for entire patch.
	if(gl_InvocationID == 0)
	{
		gl_TessLevelOuter[0] = tessLevel;
		gl_TessLevelOuter[1] = tessLevel;
		gl_TessLevelOuter[2] = tessLevel;
		gl_TessLevelOuter[3] = tessLevel;

		gl_TessLevelInner[0] = tessLevel;
		gl_TessLevelInner[1] = tessLevel;
	}

	//Passing attributes forward.
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	tcs_out[gl_InvocationID] = textureCoord[gl_InvocationID];
}