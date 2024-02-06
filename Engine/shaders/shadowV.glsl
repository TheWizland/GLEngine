#version 430

layout (location=0) in vec3 vertex;
//Probably needs to include heightmap texture to appropriately support terrain.


uniform mat4 shadowMVP;

void main(void) 
{
	gl_Position = shadowMVP * vec4(vertex, 1.0);
}