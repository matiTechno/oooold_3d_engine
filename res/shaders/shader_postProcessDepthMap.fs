#version 330

uniform sampler2D tex;

in vec2 texCoord0;

out vec4 color;

void main()
{
	float depthValue = texture(tex, texCoord0).r;	
	color = vec4(vec3(depthValue), 1);
}