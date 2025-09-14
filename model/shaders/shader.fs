#version 330 core

out
vec4 FragColor;

in
vec2 TexCoords;

in vec3 Normals;

uniform sampler2D texture_diffuse1;
uniform vec3 liColor;
uniform vec3 LightPos;
uniform vec3 FragPos;

void main()
{
    float ambientS = 0.1;
    vec3 ambient = liColor * ambientS;

    vec3 norm = normalize(Normals);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir),0.0);
    vec3 diffuse = liColor * diff;

    vec3 obColor = vec3(texture(texture_diffuse1, TexCoords)) ;
    vec3 res = (ambient + diffuse) * obColor;
    FragColor = vec4(res, 1.0);
}