#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 Normal;
out mat3 Model;
out vec2 TexCoords;
out vec3 FragPos;
out vec4 DebugColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D heightMap;

void main()
{
	vec4 texel = texture2D(heightMap, texCoord);
	vec3 displaced = position;

	//displaced.y += texel.r * 4;

    gl_Position = projection * view *  model * vec4(displaced, 1.0f);

    FragPos = vec3(model * vec4(displaced, 1.0f));
    //vec4 normalTexel = texture2D(normalMap, texCoord);
    //Normal = mat3(transpose(inverse(model))) * normalTexel.xyz;

    DebugColor = vec4(texCoord, 0, 1);

    TexCoords = texCoord;
    Model = mat3(transpose(inverse(model)));
}