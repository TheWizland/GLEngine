#version 430

in vec2 textureCoordinate;
layout (binding = 0) uniform sampler2D samp;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;

out vec4 color;

void main(void) {
    vec4 texColor = texture(samp, textureCoordinate);
    color = texColor;
    //color = vec4(1,0,0,0);
}