#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 textureCoordinate;
layout (binding = 0) uniform sampler2D samp;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;


void main(void) {
	gl_Position = proj_matrix * v_matrix * m_matrix * vec4(position, 1);
	textureCoordinate = texCoord;
}