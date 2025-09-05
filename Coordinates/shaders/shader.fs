#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
//uniform float time;
uniform float mixValue;

void main()
{
    //float r = abs(sin(time + ourColor.r));
    //float g = abs(sin(time + ourColor.g));
    //float b = abs(sin(time + ourColor.b));
    //vec3 color = vec3(r, g, b);
    float alpha = abs(sin(mixValue));
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(TexCoord.x, TexCoord.y)), alpha); //* vec4(color, 1.0);
}