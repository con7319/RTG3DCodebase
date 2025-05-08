#version 450 core

// Input vertex attributes
layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aNormal;
layout(location = 4) in vec3 aTangent;
layout(location = 5) in vec3 aBitangent;

// Output to fragment shader
out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 fragPosition;
out mat3 TBN;

// Uniforms
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform float time;

// Noise function to simulate ant-like movement
float noise(vec3 p) {
   return fract(sin(dot(p, vec3(12.9898, 78.233, 45.164))) * 43758.5453);
}

void main()
{
   // Transform vertex position to world space
   vec4 worldPosition = modelMatrix * vec4(aPos, 1.0);

   // Apply a wavy distortion to simulate movement under the surface
   float wave = sin(worldPosition.x * 10.0 + time * 5.0) * 0.1;
   float noiseEffect = noise(worldPosition.xyz + time) * 0.1;
   worldPosition.y += wave + noiseEffect;

   // Pass transformed position to the fragment shader
   fragPosition = worldPosition.xyz;

   // Calculate TBN matrix for normal mapping
   vec3 T = normalize(mat3(modelMatrix) * aTangent);
   vec3 B = normalize(mat3(modelMatrix) * aBitangent);
   vec3 N = normalize(mat3(modelMatrix) * aNormal);
   TBN = mat3(T, B, N);

   // Pass texture coordinates and normal to the fragment shader
   fragTexCoord = aTexCoord;
   fragNormal = N;

   // Transform position to clip space
   gl_Position = projMatrix * viewMatrix * worldPosition;
}
