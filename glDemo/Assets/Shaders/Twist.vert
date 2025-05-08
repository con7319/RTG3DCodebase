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
   // Apply a twisting effect around the Y-axis
   float angle = aPos.y * 2.0 + time; // Twist based on Y position and time
   float sinAngle = sin(angle);
   float cosAngle = cos(angle);

   vec3 twistedPosition = vec3(
       cosAngle * aPos.x - sinAngle * aPos.z,
       aPos.y,
       sinAngle * aPos.x + cosAngle * aPos.z
   );

   // Transform the twisted position to clip space
   fragPosition = vec3(modelMatrix * vec4(twistedPosition, 1.0));
   fragNormal = mat3(transpose(inverse(modelMatrix))) * aNormal;
   fragTexCoord = aTexCoord;

   gl_Position = projMatrix * viewMatrix * vec4(fragPosition, 1.0);
}
