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
  // Transform vertex position
  vec4 worldPosition = modelMatrix * vec4(aPos, 1.0);
  fragPosition = worldPosition.xyz;

  // Calculate normal in world space
  vec3 worldNormal = normalize(mat3(modelMatrix) * aNormal);
  fragNormal = worldNormal;

  // Calculate TBN matrix
  vec3 T = normalize(mat3(modelMatrix) * aTangent);
  vec3 B = normalize(mat3(modelMatrix) * aBitangent);
  TBN = mat3(T, B, worldNormal);

  // Apply swirl effect to texture coordinates in the opposite direction
  float radius = length(aTexCoord - vec2(0.5));
  float angle = atan(aTexCoord.y - 0.5, aTexCoord.x - 0.5) - time * 0.5; // Subtract time to reverse direction
  vec2 swirlTexCoord = vec2(0.5) + radius * vec2(cos(angle), sin(angle));
  fragTexCoord = swirlTexCoord;

  // Final vertex position
  gl_Position = projMatrix * viewMatrix * worldPosition;
}
