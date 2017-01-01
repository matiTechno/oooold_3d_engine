#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 LVP;

out vec3 fragPos;
out vec3 normal0;
out vec2 texCoord0;
out vec4 LVP_fragPos;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1);
	fragPos = vec3(model * vec4(position, 1));
	normal0 = mat3(transpose(inverse(model))) * normal;
	texCoord0 = texCoord;
	LVP_fragPos = LVP * vec4(fragPos, 1);
}