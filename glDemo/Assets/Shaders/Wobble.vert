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

// Uniforms  
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform float time;  

void main()  
{  
  // Wobble effect using sine wave  
  float wobbleFactorX = sin(aPos.y * 5.0 + time * 2.0) * 0.1;  
  float wobbleFactorZ = cos(aPos.x * 5.0 + time * 2.0) * 0.1;  
  vec3 wobblePosition = aPos + vec3(wobbleFactorX, 0.0, wobbleFactorZ);  

  fragNormal = mat3(transpose(inverse(modelMatrix))) * aNormal;
  fragPosition = vec3(modelMatrix * vec4(wobblePosition, 1.0));
  // Transform the vertex position  
  gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(wobblePosition, 1.0);  

  // Pass texture coordinates to fragment shader  
  fragTexCoord = aTexCoord;  
}
