#version 330 core
in
vec2 TexCoord;
in 
vec3 Normal;
in 
vec3 FragPos;


out
vec4 FragColor;

//uniform sampler2D ourtexture;
uniform vec3 obColor;
uniform vec3 liColor;
uniform vec3 lightPos;

void main()
{
    
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * liColor;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * liColor;
    vec3 res = (ambient + diffuse)*obColor;
    FragColor = vec4(res,1.0);
}