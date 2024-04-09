#version 430

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 texCoord;
layout (binding = 0) uniform sampler2D samp;

uniform vec3 camera_up;
uniform vec3 camera_right;
uniform mat4 m_matrix;
uniform mat4 vp_matrix;
uniform vec2 billboard_size;
out vec2 textureCoordinate;

void main(void)
{
    gl_Position = vec4(
        camera_right * vertexPosition.x * billboard_size.x
        + camera_up * vertexPosition.y * billboard_size.y, 1.0);
    gl_Position = vp_matrix * m_matrix * gl_Position;

    textureCoordinate = texCoord;
}