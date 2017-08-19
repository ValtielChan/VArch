#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 color;


out vec3 Normal;
out vec2 TexCoords;
out vec3 FragPos;
out vec4 FragPosLightSpace;
out vec3 Color;
out vec4 DebugColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D heightMap;
uniform sampler2D normalMap;

uniform mat4 lightSpaceMatrix;

void main()
{
	vec4 texel = texture2D(heightMap, texCoord);
	vec3 displaced = position;

	displaced.y += texel.r * 4;

    gl_Position = projection * view *  model * vec4(displaced, 1.0f);

    FragPos = vec3(model * vec4(displaced, 1.0f));
    Normal = texture2D(normalMap, texCoord).xyz;

    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

    Color = vec3(0, 1, 0);

    DebugColor = vec4(1, 0, 0, 1);
    TexCoords = texCoord;
}