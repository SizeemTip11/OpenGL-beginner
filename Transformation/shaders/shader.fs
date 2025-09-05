#version 330 core

out
vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;

void main()
{
    float Value= (sin(mixValue) + 1.0) / 2.0; // Normalize to [0, 1]
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), Value);
}