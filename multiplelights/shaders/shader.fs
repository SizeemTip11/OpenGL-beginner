#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct SpotLight {
    vec3 pos;
    vec3 dir;
    float inner;
    float outer;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirLight {
    vec3 dir;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 pos;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 FragPos);

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform DirLight dirLight;
#define number 4
uniform PointLight pointLights[number];
uniform SpotLight spotLight;
uniform vec3 viewPos;
uniform Material material;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 res = calcDirLight(dirLight, norm, viewDir);

    for(int i=0; i<number; i++)
    {
        res += calcPointLight(pointLights[i], norm, viewDir, FragPos);
    }

    res += calcSpotLight(spotLight, norm, viewDir, FragPos);

    FragColor = vec4(res,1.0);
}
vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.dir);
    
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

    return (ambient + diffuse + specular);
}
vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos)
{
    vec3 lightDir = normalize(light.pos - FragPos);
    
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

    float distance = length(light.pos - FragPos);
    float attenuation = 1.0/(light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 FragPos)
{
    vec3 lightDir = normalize(light.pos - FragPos);
    
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

    float theta = dot(lightDir, normalize(-light.dir));
    float epsilon = light.inner - light.outer;
    float intensity = clamp((theta - light.inner)/epsilon, 0.0, 1.0);

    float distance = length(light.pos - FragPos);
    float attenuation = 1.0/(light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

    diffuse *= intensity;
    specular *= intensity;
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}