//normal map vertex shader
#version 450 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

layout (location=0) in vec3 vertexPos;
layout (location=1) in vec3 vertexTangent;
layout (location=2) in vec2 vertexTexCoord;
layout (location=3) in vec3 vertexNormal;

out SimplePacket {
    vec3 surfaceWorldPos;
    vec3 surfaceNormal;
    vec2 texCoord;
    vec3 T;
    vec3 B;
    vec3 N;
} outputVertex;

void main(void) {
    outputVertex.texCoord = vertexTexCoord;

    // Transform normal, tangent, and compute bitangent
    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));

    vec3 N = normalize(normalMatrix * vertexNormal);
    vec3 T = normalize(normalMatrix * vertexTangent);
    vec3 B = cross(N, T); // Compute bitangent

    outputVertex.N = N;
    outputVertex.T = T;
    outputVertex.B = B;

    // Transform position
    vec4 worldCoord = modelMatrix * vec4(vertexPos, 1.0);
    outputVertex.surfaceWorldPos = worldCoord.xyz;

    gl_Position = projMatrix * viewMatrix * worldCoord;
}

