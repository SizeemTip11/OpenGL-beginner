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

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

struct Light {
    vec3 pos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

void main()
{
    vec3 ambient = light.ambient * material.ambient;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.pos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir),0.0),64);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 res = ambient + diffuse + specular;
    FragColor = vec4(res,1.0);
}