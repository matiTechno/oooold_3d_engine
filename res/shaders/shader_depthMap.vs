#version 330

layout (location=0) in vec3 position;

uniform mat4 model;
uniform mat4 LVP;

void main()
{
	gl_Position = LVP * model * vec4(position, 1);
}