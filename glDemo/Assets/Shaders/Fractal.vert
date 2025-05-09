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
out vec3 blendedColor;  

uniform mat4 modelMatrix;  
uniform mat4 viewMatrix;  
uniform mat4 projMatrix;  
uniform float time;  
uniform sampler2D texture1;  
uniform sampler2D texture2;  
uniform sampler2D normalMap;  

void main()  
{  
   // Apply a wavy effect to the vertex position  
   vec3 animatedPos = aPos;  
   animatedPos.y += sin(animatedPos.x * 5.0 + time) * 0.2;  
   animatedPos.z += cos(animatedPos.y * 5.0 + time) * 0.2;  

   // Transform position to world space  
   surfaceWorldPos = vec3(modelMatrix * vec4(animatedPos, 1.0));  

   // Calculate TBN matrix for normal mapping  
   vec3 T = normalize(mat3(modelMatrix) * aTangent);  
   vec3 B = normalize(mat3(modelMatrix) * aBitangent);  
   vec3 N = normalize(mat3(modelMatrix) * aNormal);  
   TBN = mat3(T, B, N);  

   // Sample the normal map and adjust the surface normal  
   vec3 normalFromMap = texture(normalMap, aTexCoord).rgb * 2.0 - 1.0;  
   surfaceNormal = normalize(TBN * normalFromMap);  

   texCoord = aTexCoord;  

   // Blend two textures based on a sine wave effect  
   float blendFactor = 0.5 + 0.5 * sin(time + aPos.x * 10.0);  
   vec3 color1 = texture(texture1, texCoord).rgb;  
   vec3 color2 = texture(texture2, texCoord).rgb;  
   blendedColor = mix(color1, color2, blendFactor);  

   // Transform to clip space  
   gl_Position = projMatrix * viewMatrix * vec4(surfaceWorldPos, 1.0);  
}