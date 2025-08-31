#version 330 core
in vec3 ourColor; // Input color from vertex shader
out vec4 FragColor; // Output color to the framebuffer
uniform float time; // Uniform variable for time
void main()
{
    float r = abs(sin(time+ourColor.r));
    float g = abs(sin(time+ourColor.g));
    float b = abs(sin(time+ourColor.b));   
    FragColor = vec4(r, g, b, 1.0); // Set the output color
}