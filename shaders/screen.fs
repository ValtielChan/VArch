#version 430
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;

void main()
{ 
	vec4 texel = texture(screenTexture, TexCoords);
	float depth = texture(screenTexture, TexCoords).r;

    color = vec4(texel.x, texel.y, texel.z, 1);
    color = vec4(vec3(depth), 1);
}