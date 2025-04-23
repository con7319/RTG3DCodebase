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

 uniform float texScale;

 void main()
 {
    // Scale the texture coordinates to allow the whole texture to be shown on each face
    vec2 scaledTexCoord = inputFragment.texCoord * texScale;

    // Sample the normal map to get the normal vector
    vec3 normal = texture(normalMap, scaledTexCoord).rgb;

    // Perform lighting calculations using the normal
    FragColor = vec4(normal, 1.0);
 }