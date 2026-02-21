#version 330 core
layout (location = 0) in vec3 aPos;    // vertex position
layout (location = 1) in vec3 aNormal; // vertex normal

out vec3 FragPos;   // position in world space
out vec3 Normal;    // normal in world space

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));   // world-space position
    Normal = mat3(transpose(inverse(model))) * aNormal; // transform normal correctly
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
