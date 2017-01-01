#version 330

layout(location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 view;

out vec3 texCoord0;

void main()
{
	gl_Position = (projection * mat4(mat3(view)) * vec4(position, 1)).xyww;
	texCoord0 = vec3(position.x, position.y, position.z);
}