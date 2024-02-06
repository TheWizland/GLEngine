#version 430

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 vertexNormal;
layout (binding = 0) uniform sampler2D samp;
layout (binding = 1) uniform sampler2D height;
layout (binding = 2) uniform sampler2DShadow shadowTex;
out vec2 textureCoordinate;
out vec3 varyingNormal;
out vec3 varyingVertexPosition;
out vec4 shadow_coord;

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
uniform mat4 vp_shadow;
uniform vec4 globalAmbient;

uniform int internallyLit; //-1 if internally lit, 1 if not
uniform int hasShadows; //0 if shadows are cast on this object, 1 if no shadows

uniform int heightMapped; //0 if no height map, 1 if height map
vec3 heightChange;
float heightMapMult = 2.f;

void main(void) {
	heightChange = vertexNormal*texture(height, texCoord).r * heightMapMult * heightMapped;

	varyingVertexPosition = (m_matrix * vec4(vertexPosition, 1.0)).xyz;
    varyingNormal = (norm_matrix * vec4(vertexNormal, 1.0)).xyz;
	varyingNormal = internallyLit * varyingNormal;

	gl_Position = vec4(vertexPosition + heightChange, 1);

	shadow_coord = vp_shadow * m_matrix * gl_Position;

	gl_Position = proj_matrix * v_matrix * m_matrix * gl_Position;
	textureCoordinate = texCoord;
}