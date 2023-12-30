#version 430

uniform float offsetX = 0;
uniform float offsetY = 0;
uniform float rotationRadians = 0;
uniform float scale = 1;
bool gradientEnabled = true;
float xCoord[3] = {0.1, -0.1, 0.0};
float yCoord[3] = {-0.25, -0.25, 0.25};
vec4 colorArr[3] = {vec4(1,0,0,1), vec4(0,1,0,1), vec4(0,0,1,1)};
vec4 defaultColor = vec4(0, 0, 1, 1);

out vec4 varyingColor;

void main(void) {
	float xPos = scale*(xCoord[gl_VertexID]*cos(rotationRadians)-yCoord[gl_VertexID]*sin(rotationRadians));
	float yPos = scale*(yCoord[gl_VertexID]*cos(rotationRadians)+xCoord[gl_VertexID]*sin(rotationRadians));
	gl_Position = vec4(xPos+offsetX, yPos+offsetY, 0, 1.0);
	/*float xPos = xCoord[gl_VertexID];
	float yPos = yCoord[gl_VertexID];
	gl_Position = vec4(xPos, yPos, 0, 1.0);*/
	
	if(gradientEnabled)
		varyingColor = colorArr[gl_VertexID];
	else
		varyingColor = vec4(0,0,1,1);
}