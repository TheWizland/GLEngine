#version 430

layout (location = 0) in vec3 position;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;

out vec4 varyingColor;

void main(void) {
	gl_Position = proj_matrix * v_matrix * m_matrix * vec4(position, 1);
	varyingColor = vec4(0,0,1,1);
}