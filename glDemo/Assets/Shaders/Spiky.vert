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

void main()
{
   // Generate a pseudo-random noise based on vertex position
   float noise = fract(sin(dot(aPos.xyz, vec3(12.9898, 78.233, 45.164))) * 43758.5453);

   // Add time-based variation to the noise
   float bumpFactor = noise * (sin(time * 5.0) * 0.5 + 0.5);

   // Smooth the bump effect to reduce seams and pulsation
   bumpFactor = smoothstep(0.0, 1.0, bumpFactor);

   // Apply the bump effect to the vertex position
   vec3 bumpedPosition = aPos + normalize(aNormal) * bumpFactor * 0.3;

   // Transform the modified position to world space
   vec4 worldPosition = modelMatrix * vec4(bumpedPosition, 1.0);
   fragPosition = worldPosition.xyz;

   // Pass texture coordinates and normal to the fragment shader
   fragTexCoord = aTexCoord;
   fragNormal = normalize(mat3(transpose(inverse(modelMatrix))) * aNormal); // Normalize transformed normal

   // Calculate TBN matrix for normal mapping
   vec3 T = normalize(mat3(modelMatrix) * aTangent);
   vec3 B = normalize(mat3(modelMatrix) * aBitangent);
   vec3 N = normalize(fragNormal);
   TBN = mat3(T, B, N);

   // Final position in clip space
   gl_Position = projMatrix * viewMatrix * worldPosition;
}
