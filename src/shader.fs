#version 330 core
in vec3 Normal;
in vec3 FragPos;

out vec4 outColor;

uniform vec3 lightColor;
uniform vec3 objectColor;
in vec2 TexCoord;

uniform sampler2D texture2;

void main()
{
    vec3 ambient = vec3(0.2, 0.2, 0.2);
    vec3 norm = normalize(Normal);
    vec3 lightPos = vec3(5.0, 5.0, 5.0);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 diffuse = vec3(0.5, 0.5, 0.5) * max(dot(norm, lightDir), 0.0);
    vec3 specular = vec3(0.8, 0.8, 0.8) * pow(max(dot(reflect(-lightDir, norm), normalize(FragPos)), 0.0), 32);

    outColor = texture(texture2, TexCoord);
}