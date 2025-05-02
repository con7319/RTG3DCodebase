#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aNormal;
layout(location = 4) in vec3 aTangent;
layout(location = 5) in vec3 aBitangent;

out vec3 surfaceWorldPos;
out vec3 surfaceNormal;
out vec2 texCoord;
out mat3 TBN; // Tangent, Bitangent, Normal

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main()
{
  vec4 worldPos = modelMatrix * vec4(aPos, 1.0);
  surfaceWorldPos = worldPos.xyz;
  texCoord = aTexCoord;

  vec3 T = normalize(vec3(modelMatrix * vec4(aTangent, 0.0)));
  vec3 B = normalize(vec3(modelMatrix * vec4(aBitangent, 0.0)));
  vec3 N = normalize(vec3(modelMatrix * vec4(aNormal, 0.0)));

  TBN = mat3(T, B, N);
  

  surfaceNormal = normalize(vec3(mat3(modelMatrix) * aNormal));

  gl_Position = projMatrix * viewMatrix * worldPos;
}