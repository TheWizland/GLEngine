#version 430

in vec4 varyingColor;
out vec4 color;
void main(void) {
    color = varyingColor;
}