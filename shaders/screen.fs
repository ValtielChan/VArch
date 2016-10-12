#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;

void main()
{ 
	vec4 texel = texture(screenTexture, TexCoords);
    color = vec4(texel.x, texel.y, texel.z, 1);
    //color = vec4(1, 0, 1, 1);
}