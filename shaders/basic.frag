#version 330 core
in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightDir;   // direction FROM sun TO scene
uniform vec3 lightColor;
uniform vec3 objectColor;

out vec4 FragColor;

void main() {
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, -lightDir), 0.0); // note the negative, light comes TO object
    vec3 diffuse = diff * lightColor;
    
    vec3 ambient = 0.1 * lightColor; // small ambient
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}
