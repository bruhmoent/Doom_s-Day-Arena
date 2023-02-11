#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;

uniform vec3 viewPos;
uniform vec3 viewDir;
uniform vec3 cameraFront;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));

    vec3 lightPos = vec3(5.0, 5.0, 5.0);
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    vec3 ambient = vec3(0.2, 0.2, 0.2);
    vec3 diffuse = vec3(0.5, 0.5, 0.5) * max(dot(norm, lightDir), 0.0);
    vec3 specular = vec3(0.8, 0.8, 0.8) * spec;

    TexCoord = aTexCoord;
}