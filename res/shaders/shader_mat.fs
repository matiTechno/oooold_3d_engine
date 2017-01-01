#version 330

#define num_sunLights 1
#define num_pointLights 3
#define num_spotLights 1

uniform sampler2D depthMap;

uniform vec3 camPosition;
uniform vec3 mat_ambient;
uniform vec3 mat_diffuse;
uniform vec3 mat_specular;
uniform float mat_shininess;

in vec3 fragPos;
in vec3 normal0;
in vec2 texCoord0;
in vec4 LVP_fragPos;

out vec4 color;

struct SunLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float a;
	float b;
	float c;
};

struct SpotLight
{
	vec3 direction;
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float a;
	float b;
	float c;
	float innerAngle;
	float outerAngle;
};

#if num_sunLights
uniform SunLight sunLights[num_sunLights];
#endif
#if num_pointLights
uniform PointLight pointLights[num_pointLights];
#endif
#if num_spotLights
uniform SpotLight spotLights[num_spotLights];
#endif

vec3 add_sunLight(SunLight light, vec3 viewDir, vec3 normal);
vec3 add_pointLight(PointLight light, vec3 viewDir, vec3 normal);
vec3 add_spotLight(SpotLight light, vec3 viewDir, vec3 normal);

float shadowCalc()
{
	float bias = 0.005;
	
	vec3 projCoords = LVP_fragPos.xyz / LVP_fragPos.w;
	projCoords = projCoords * 0.5 + 0.5;
	float currentDepth = projCoords.z;

	float shadow = 0;
	
	vec2 texelSize = 1 / vec2(textureSize(depthMap, 0));
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(depthMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += (currentDepth - bias) > pcfDepth ? 1 : 0;
		}
	}
	shadow /= 9;	
	if(currentDepth > 1)
		shadow = 0;
	return shadow;
}

void main()

{
	vec3 viewDir = normalize(fragPos - camPosition);
	vec3 normal = normalize(normal0);
	vec3 result = vec3(0, 0, 0);

	#if num_sunLights
	for(int i = 0; i < num_sunLights; i++)
	{result += add_sunLight(sunLights[i], viewDir, normal);}
	#endif

	#if num_pointLights
	for(int i = 0; i < num_pointLights; i++)
	{result += add_pointLight(pointLights[i], viewDir, normal);}
	#endif

	#if num_spotLights
	for(int i = 0; i < num_spotLights; i++)
	{result += add_spotLight(spotLights[i], viewDir, normal);}
	#endif

	color = vec4(result, 1);
}


vec3 add_sunLight(SunLight light, vec3 viewDir, vec3 normal)
{
	vec3 minusLightDir = -normalize(light.direction);

	float diff_coeff = max(dot(normal, minusLightDir), 0);

	vec3 H = normalize(minusLightDir + -viewDir);
	float spec_coeff = pow(max(dot(normal, H), 0), mat_shininess) * diff_coeff;

	float shadow = shadowCalc();

	vec3 ambient = mat_ambient * light.ambient;
	vec3 diffuse = mat_diffuse * light.diffuse * diff_coeff * (1 - shadow);
	vec3 specular = mat_specular * light.specular * spec_coeff * (1 - shadow);

	return (ambient + diffuse + specular);
}


vec3 add_pointLight(PointLight light, vec3 viewDir, vec3 normal)
{
	vec3 minusLightDir = normalize(light.position - fragPos);

	float diff_coeff = max(dot(normal, minusLightDir), 0);

	vec3 H = normalize(minusLightDir + -viewDir);
	float spec_coeff = pow(max(dot(normal, H), 0), mat_shininess) * diff_coeff;

	float mod_Frag_to_Light = length(light.position - fragPos);
	float attenuation = 1.0/(light.a*mod_Frag_to_Light*mod_Frag_to_Light + light.b*mod_Frag_to_Light + light.c*mod_Frag_to_Light);

	vec3 ambient = mat_ambient * light.ambient * attenuation;
	vec3 diffuse = mat_diffuse * light.diffuse * diff_coeff * attenuation;
	vec3 specular = mat_specular * light.specular * spec_coeff * attenuation;

	return (ambient + diffuse + specular);
}


vec3 add_spotLight(SpotLight light, vec3 viewDir, vec3 normal)
{
	vec3 minusLightDir = normalize(light.position - fragPos);

	float diff_coeff = max(dot(normal, minusLightDir), 0);

	vec3 H = normalize(minusLightDir + -viewDir);
	float spec_coeff = pow(max(dot(normal, H), 0), mat_shininess) * diff_coeff;

	float mod_Frag_to_Light = length(light.position - fragPos);
	float attenuation = 1.0/(light.a*mod_Frag_to_Light*mod_Frag_to_Light + light.b*mod_Frag_to_Light + light.c*mod_Frag_to_Light);

	float theta = dot(minusLightDir, -normalize(light.direction));
	float spot_coeff = clamp((theta - light.outerAngle)/(light.innerAngle - light.outerAngle), 0, 1);

	vec3 ambient = mat_ambient * light.ambient * attenuation;
	vec3 diffuse = mat_diffuse * light.diffuse * diff_coeff * attenuation * spot_coeff;
	vec3 specular = mat_specular * light.specular * spec_coeff * attenuation * spot_coeff;

	return (ambient + diffuse + specular);
}