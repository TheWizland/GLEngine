#version 430

in vec3 tc;
out vec4 fragColor;

uniform mat4 v_matrix;
uniform mat4 proj_matrix;
layout(binding = 0) uniform samplerCube samp;

/*vec4 fogColor = vec4(0.7, 0.8, 0.9, 1);
float fogEnd = -0.75;
float fogStart = 0.4;*/

void main(void)
{
    fragColor = texture(samp, tc);

    /*float dist = tc[1];
    float fogFactor = clamp(((fogEnd-dist)/(fogEnd-fogStart)), 0.0, 1.0);
    fragColor = mix(fogColor, fragColor, fogFactor);*/
}
