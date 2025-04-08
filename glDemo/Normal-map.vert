#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aNormal;
layout(location = 4) in vec3 aTangent;
layout(location = 5) in vec3 aBitangent;

out SimplePacket {
    vec3 surfaceWorldPos;
    vec3 surfaceNormal;
    vec2 texCoord;
    vec3 T;  // Tangent
    vec3 B;  // Bitangent
    vec3 N;  // Normal
} outputFragment;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    outputFragment.surfaceWorldPos = worldPos.xyz;
    outputFragment.surfaceNormal = normalize(mat3(transpose(inverse(model))) * aNormal);
    outputFragment.texCoord = aTexCoord;
    outputFragment.T = normalize(mat3(transpose(inverse(model))) * aTangent);
    outputFragment.B = normalize(mat3(transpose(inverse(model))) * aBitangent);
    outputFragment.N = normalize(mat3(transpose(inverse(model))) * aNormal);

    gl_Position = projection * view * worldPos;
}