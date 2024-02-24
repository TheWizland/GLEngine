#version 430

in vec2 textureCoordinate;
in vec3 varyingNormal;
in vec3 varyingTangent;
in vec3 varyingVertexPosition;
in vec4 shadow_coord;
layout (binding = 0) uniform sampler2D samp;
layout (binding = 1) uniform sampler2D height;
layout (binding = 2) uniform sampler2DShadow shadowTex;
layout (binding = 3) uniform sampler2D normalMap;
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

uniform int internallyLit; //-1 if internally lit, 1 if not
uniform int hasShadows; //0 if shadows are cast on this object, 1 if no shadows
uniform int heightMapped; //0 if no height map, 1 if height map
uniform int normalMapped;

vec3 L, N, V, R;
vec4 ambient, diffuse, specular;
vec3 halfVector;
float cosTheta, cosPhi;
float dist, attenuationFactor, intensity;
vec4 texColor;

float lookup(float x, float y) {
    float texVal = textureProj(shadowTex, shadow_coord + vec4(x * 0.001 * shadow_coord.w,
                                                         y * 0.001 * shadow_coord.w,
                                                         -0.01, 0.0));
    return texVal;
}

vec3 calcNewNormal() {
    vec3 normal = N;
    vec3 tangent = normalize(varyingTangent);
    tangent = normalize(tangent - dot(tangent, normal) * normal);
    vec3 bitangent = cross(tangent, normal);
    mat3 tbn = mat3(tangent, bitangent, normal);
    vec3 retrievedNormal = texture(normalMap,textureCoordinate).xyz;
    retrievedNormal = retrievedNormal * 2.0 - 1.0;
    vec3 newNormal = tbn * retrievedNormal;
    newNormal = normalize(newNormal);
    return newNormal;
}

void main(void) {
    N = normalize(varyingNormal);
    if(normalMapped == 1)
        N = calcNewNormal();

	L = normalize(light.position - varyingVertexPosition);
	
	V = normalize(-v_matrix[3].xyz - varyingVertexPosition);

	cosTheta = dot(L,N);
	halfVector = normalize(L + V);
    cosPhi = dot(halfVector,N);

	dist = distance(varyingVertexPosition, light.position);
	attenuationFactor = 1.0 / (light.constantAttenuation + light.linearAttenuation*dist + light.quadraticAttenuation*dist*dist);

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
    shadowFactor = min(shadowFactor + hasShadows, 1.0);

    texColor = texture(samp, textureCoordinate);
	color = texColor * (ambient + shadowFactor*(diffuse + specular));
	//Alternatively:
    //color = texColor * (ambient + diffuse) + specular;
}