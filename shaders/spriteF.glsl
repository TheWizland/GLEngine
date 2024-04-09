#version 430

layout (binding = 0) uniform sampler2D samp;

uniform vec3 camera_up;
uniform vec3 camera_right;
uniform mat4 m_matrix;
uniform mat4 vp_matrix;
uniform vec2 billboard_size;
in vec2 textureCoordinate;
out vec4 color;

void main(void)
{
    color = texture(samp, textureCoordinate);
}