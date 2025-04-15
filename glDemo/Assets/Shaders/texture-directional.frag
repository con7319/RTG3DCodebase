#version 450 core

// Diffuse texture - directional light

// Texture sampler (for diffuse surface colour)
layout(binding = 0) uniform sampler2D texture;

// Directional light model
struct dirLight {
    vec3 d_pos;
    vec3 d_col;
    vec3 d_amb;
    vec3 d_dir; 
};

struct pointLight {
    vec3 p_pos;
    vec3 p_col;
    vec3 p_attenuation; // x = constant, y = linear, z = quadratic
};

struct spotLight {
    vec3 s_pos;
    vec3 s_col;
    vec3 s_att; // x = constant, y = linear, z = quadratic
    vec3 s_dir;
    float s_cutoff;
    float s_oCutoff;
};

uniform dirLight dirLights[10];
uniform int numDirLights;

uniform pointLight pointLights[10];
uniform int numPointLights;

uniform spotLight spotLights[10];
uniform int numSpotLights;

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
    vec3 finalColor = vec3(0.0);
    if (numDirLights > 0) {
        vec3 totalAmbient = vec3(0.0);
        for (int i = 0; i < numDirLights; ++i) {
            vec3 lightDir = normalize(-dirLights[i].d_dir);
            float l = max(dot(N, lightDir), 0.0);
            vec3 diffuseColor = surfaceColor.rgb * dirLights[i].d_col * l;
            totalAmbient += dirLights[i].d_amb;
            finalColor += diffuseColor;
        }
        finalColor += totalAmbient;
    }

    // Point light calculation
    if (numPointLights > 0) {
        for (int i = 0; i < numPointLights; ++i) {
            vec3 surfaceToLightVec = pointLights[i].p_pos - inputFragment.surfaceWorldPos;
            vec3 surfaceToLightNormalized = normalize(surfaceToLightVec);
            float Pl = max(dot(N, surfaceToLightNormalized), 0.0);

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
    }

    // Spot light calculation
    if (numSpotLights > 0) {
        for (int i = 0; i < numSpotLights; ++i) {
            vec3 surfaceToLightVec = spotLights[i].s_pos - inputFragment.surfaceWorldPos;
            vec3 surfaceToLightNormalized = normalize(surfaceToLightVec);
            float Sl = max(dot(N, surfaceToLightNormalized), 0.0);

            float d = length(surfaceToLightVec);
            float kc = spotLights[i].s_att.x;
            float kl = spotLights[i].s_att.y;
            float kq = spotLights[i].s_att.z;
            float attenuation = 1.0 / (kc + kl * d + kq * d * d);

            vec3 spotDir = normalize(-spotLights[i].s_dir);
            float theta = dot(surfaceToLightNormalized, spotDir);
            float epsilon = spotLights[i].s_cutoff - spotLights[i].s_oCutoff;
            float intensity = clamp((theta - spotLights[i].s_oCutoff) / epsilon, 0.0, 1.0);

            vec3 SdiffuseColor = surfaceColor.rgb * spotLights[i].s_col * Sl * attenuation * intensity;
            finalColor += SdiffuseColor;
        }
    }

    fragColour = vec4(finalColor, surfaceColor.a);
}
