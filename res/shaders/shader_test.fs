#version 330

uniform vec3 simpleColor;

out vec4 color;

void main()

{
	color = vec4(simpleColor, 1);
}