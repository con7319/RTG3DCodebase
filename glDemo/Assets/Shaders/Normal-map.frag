#version 450 core

layout(binding = 0) uniform sampler2D baseTexture;
layout(binding = 1) uniform sampler2D normalMap;

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

in vec3 surfaceWorldPos;
in vec3 surfaceNormal;
in vec2 texCoord;
in mat3 TBN; // Tangent, Bitangent, Normal

out vec4 FragColour;

uniform float texScale;

void main()
{
  // Scale the texture coordinates
  vec2 scaledTexCoord = texCoord * texScale;

  // Sample the normal map and transform it using the TBN matrix
  vec3 normal = texture(normalMap, scaledTexCoord.xy).rgb;
  normal = normal * 2.0 - 1.0;
  normal = normalize(TBN * normal);
  normal = -normal;

  // Sample the base texture for the surface color
  vec4 surfaceColor = texture(baseTexture, scaledTexCoord.xy);
  float alpha = surfaceColor.a;

  // Initialize final color
  vec3 finalColour = vec3(0.0);

  // Directional light calculation
  if (numDirLights > 0) {
      vec3 totalAmbient = vec3(0.0);
      for (int i = 0; i < numDirLights; ++i) {
          vec3 lightDir = normalize(-dirLights[i].d_dir);
          float l = max(dot(-normal, lightDir), 0.0); // flipped normal to show on correct side
          vec3 diffuseColor = surfaceColor.rgb * dirLights[i].d_col * l;
          totalAmbient += dirLights[i].d_amb;
          finalColour += diffuseColor;
          
      }
      
  }

  // Point light calculation
  if (numPointLights > 0) {
    vec3 totalPointLightColor = vec3(0.0);
    for (int i = 0; i < numPointLights; ++i) {
        vec3 surfaceToLightVec = pointLights[i].p_pos - surfaceWorldPos;
        vec3 surfaceToLightNormalized = normalize(surfaceToLightVec);
        float Pl = max(dot(normal, surfaceToLightNormalized), 0.0);

        float d = length(surfaceToLightVec);
        float maxDistance = 8.0f;

        if (d < maxDistance) {
            float kc = pointLights[i].p_attenuation.x;
            float kl = pointLights[i].p_attenuation.y;
            float kq = pointLights[i].p_attenuation.z;
            float attenuation = 1.0 / (kc + kl * d + kq * d * d);

            vec3 PdiffuseColor = surfaceColor.rgb * pointLights[i].p_col * Pl * attenuation;
            PdiffuseColor = clamp(PdiffuseColor, 0.0, 1.0); // Clamp to avoid over-brightness

            totalPointLightColor += PdiffuseColor;
        }
    }
    finalColour += totalPointLightColor;
}

    // Spot light calculation
    if (numSpotLights > 0) {
        vec3 totalSpotLightColor = vec3(0.0);
        for (int i = 0; i < numSpotLights; ++i) {
            vec3 surfaceToLightVec = spotLights[i].s_pos - surfaceWorldPos;
            vec3 surfaceToLightNormalized = normalize(surfaceToLightVec);
            float Sl = max(dot(normal, surfaceToLightNormalized), 0.0);

            float d = length(surfaceToLightVec);
            float kc = spotLights[i].s_att.x;
            float kl = spotLights[i].s_att.y;
            float kq = spotLights[i].s_att.z;
            float attenuation = 1.0 / (kc + kl * d + kq * d * d);

            vec3 spotDir = normalize(-spotLights[i].s_dir);
            float theta = dot(-surfaceToLightNormalized, spotDir);
            float epsilon = spotLights[i].s_cutoff - spotLights[i].s_oCutoff;
            float intensity = clamp((theta - spotLights[i].s_oCutoff) / epsilon, 0.0, 1.0);

            vec3 SdiffuseColor = surfaceColor.rgb * spotLights[i].s_col * Sl * attenuation * intensity;

            // Mix the current light contribution with the accumulated color
            totalSpotLightColor = mix(totalSpotLightColor, SdiffuseColor, 0.5);
            finalColour += totalSpotLightColor;
        }
        
    }

  // Apply multi-light blending (clamping the final color to avoid over-brightness)
  finalColour = clamp(finalColour, 0.0, 1.0);

  // Output the final color
  FragColour = vec4(finalColour, alpha);
}