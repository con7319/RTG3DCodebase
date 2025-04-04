#version 450 core

layout(binding = 0) uniform sampler2D texture;
layout(binding = 1) uniform sampler2D normalMap; // Normal map sampler

uniform vec3 DIRDir;  // Light direction (normalized)
uniform vec3 DIRCol;  // Light color
uniform vec3 DIRAmb;  // Ambient light

in SimplePacket {
    vec3 surfaceWorldPos;
    vec3 surfaceNormal;
    vec2 texCoord;
    vec3 T;  // Tangent
    vec3 B;  // Bitangent
    vec3 N;  // Normal
} inputFragment;

layout (location=0) out vec4 fragColour;

void main(void) {
    // Sample normal from normal map (range [0,1])
    vec3 normalSample = texture(normalMap, inputFragment.texCoord).rgb;

    // Convert normal map from [0,1] to [-1,1]
    vec3 tangentNormal = normalize(normalSample * 2.0 - 1.0);

    // Construct TBN matrix (Tangent, Bitangent, Normal)
    mat3 TBN = mat3(normalize(inputFragment.T), normalize(inputFragment.B), normalize(inputFragment.N));

    // Transform normal from tangent space to world space
    vec3 worldNormal = normalize(TBN * tangentNormal);

    // Lambertian shading (diffuse lighting)
    float l = max(dot(worldNormal, normalize(DIRDir)), 0.0);

    // Sample diffuse texture
    vec4 surfaceColour = texture(texture, inputFragment.texCoord);
    vec3 diffuseColour = surfaceColour.rgb * DIRCol * l;

    // Final color output: ambient + diffuse
    fragColour = vec4(DIRAmb + diffuseColour, 1.0);
}