#version 330 core
layout(location = 0) in
vec3 aPos;
void main()
{
    // Output the position of the vertex
    gl_Position = vec4(aPos, 1.0);
}