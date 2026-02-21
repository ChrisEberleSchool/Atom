#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;

//
// --- High-frequency procedural noise ---
// Cheap hash noise (no texture needed)
//
float hash(vec3 p)
{
    p = fract(p * 0.3183099 + 0.1);
    p *= 17.0;
    return fract(p.x * p.y * p.z * (p.x + p.y + p.z));
}

float noise(vec3 p)
{
    vec3 i = floor(p);
    vec3 f = fract(p);
    f = f * f * (3.0 - 2.0 * f);

    float n =
        mix(mix(mix(hash(i + vec3(0,0,0)), hash(i + vec3(1,0,0)), f.x),
                mix(hash(i + vec3(0,1,0)), hash(i + vec3(1,1,0)), f.x), f.y),
            mix(mix(hash(i + vec3(0,0,1)), hash(i + vec3(1,0,1)), f.x),
                mix(hash(i + vec3(0,1,1)), hash(i + vec3(1,1,1)), f.x), f.y),
            f.z);

    return n;
}

void main()
{
    vec3 N = normalize(Normal);
    vec3 L = normalize(-lightDir);
    vec3 V = normalize(viewPos - FragPos);

    // -------------------------
    // Matte diffuse lighting
    // -------------------------
    float diff = max(dot(N, L), 0.0);

    // Soft shadow contrast (matte look)
    float shade = 0.35 + diff * 0.75;
    vec3 color = objectColor * shade;

    // -------------------------
    // High-frequency surface texture
    // -------------------------
    float n = noise(FragPos * 6.0);      // texture scale
    float micro = noise(FragPos * 25.0); // fine grain

    // Combine for HD detail
    float textureValue = n * 0.6 + micro * 0.4;

    // Apply subtle variation (matte roughness)
    color *= 0.85 + textureValue * 0.3;

    // -------------------------
    // Soft rim (cloth / clay edge)
    // -------------------------
    float rim = 1.0 - max(dot(N, V), 0.0);
    rim = smoothstep(0.6, 1.0, rim);
    color += rim * objectColor * 0.25;

    // -------------------------
    // Very slight color flattening (film-like)
    // -------------------------
    float levels = 8.0;
    color = floor(color * levels) / levels;

    FragColor = vec4(color, 0.05);
}
