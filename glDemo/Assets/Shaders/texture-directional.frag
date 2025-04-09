#version 450 core

// Diffuse texture - directional light

// Texture sampler (for diffuse surface colour)
layout(binding = 0) uniform sampler2D texture;

// Directional light model
uniform vec3 DIRDir;
uniform vec3 DIRCol;
uniform vec3 DIRAmb;

struct pointLight {
	vec3 p_pos;
	vec3 p_col;
	vec3 p_attenuation; // x = constant, y = linear, z = quadratic

};


uniform pointLight pointLights[10];
uniform int numPointLights;

in SimplePacket {
	
	vec3 surfaceWorldPos;
	vec3 surfaceNormal;
	vec2 texCoord;

} inputFragment;


layout (location=0) out vec4 fragColour;

void main(void) {
    vec3 N = normalize(inputFragment.surfaceNormal);
    vec4 surfaceColor = texture2D(texture, inputFragment.texCoord);

    // Directional light calculation
    float l = dot(N, DIRDir);
    //vec3 diffuseColor = surfaceColor.rgb * DIRCol * l;
    vec3 diffuseColor =  surfaceColor.rgb ;

    // Point light calculations
    vec3 finalColor = DIRAmb + diffuseColor;

    for (int i = 0; i < numPointLights; ++i) {
        vec3 surfaceToLightVec = pointLights[i].p_pos - inputFragment.surfaceWorldPos;
        vec3 surfaceToLightNormalized = normalize(surfaceToLightVec);
        float Pl = dot(N, surfaceToLightNormalized);

        float d = length(surfaceToLightVec);
        float maxDistance = 8.0f;

        if (d < maxDistance) {
            float kc = pointLights[i].p_attenuation.x;
            float kl = pointLights[i].p_attenuation.y;
            float kq = pointLights[i].p_attenuation.z;
            float attenuation = 1.0 / (kc + kl * d + kq * d * d);
            float smoothAttenuation = smoothstep(0.0, 1.0, attenuation);
            vec3 PdiffuseColor = surfaceColor.rgb * pointLights[i].p_col * Pl * smoothAttenuation;
            finalColor += PdiffuseColor;
        }
       
 
    }
    
    fragColour = vec4(finalColor, surfaceColor.a);
}
