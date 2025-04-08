 #version 450 core

   in SimplePacket {
       vec3 surfaceWorldPos;
       vec3 surfaceNormal;
       vec2 texCoord;
       vec3 T;  // Tangent
       vec3 B;  // Bitangent
       vec3 N;  // Normal
   } inputFragment;

   out vec4 FragColor;

   uniform sampler2D normalMap;

   void main()
   {
    
        // Sample the normal map to get the normal vector  
       vec3 normal = texture(normalMap, inputFragment.texCoord).rgb;
       // Perform lighting calculations using the normal
       FragColor = vec4(normal, 1.0);
   }