#version 330 core
out
vec4 FragColor;
uniform float time;
void main()
{
    float r=abs(sin(time));
    float g=abs(sin(time+2.0));
    float b=abs(sin(time+4.0));
    FragColor = vec4(r, g, b, 1.0);
}