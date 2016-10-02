#version 330 core

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Color;  
in vec4 DebugColor;
  
out vec4 color;

void main()
{
    color = vec4(Color, 0.0f);
} 