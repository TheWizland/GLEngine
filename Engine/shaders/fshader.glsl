#version 430

in vec4 varyingColor;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;

out vec4 color;

void main(void) {
    color = vec4(1,0,0,1);
}