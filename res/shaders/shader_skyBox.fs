#version 330

uniform samplerCube skyBox;

in vec3 texCoord0;
out vec4 color;

void main()
{
	color = texture(skyBox, texCoord0);
}