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
uniform vec3 viewPos;

void main()
{
    
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * liColor;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * liColor;
    
    float specStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir),0.0),64);
    vec3 specular = specStrength * spec * liColor;

    vec3 res = (ambient + diffuse + specular)*obColor;
    FragColor = vec4(res,1.0);
}