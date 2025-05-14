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
uniform float swirlRadiusLimit = 1.0; // Upper limit for swirl radius

void main()
{
// Compute radius and clamp to swirl limit
float radius = length(aPos.xy);
float clampedRadius = min(radius, swirlRadiusLimit);

// Swirl strength: fade out smoothly at center and edge, sharper falloff
float swirlStrength = pow(smoothstep(0.0, swirlRadiusLimit * 0.95, clampedRadius), 2.2);

// Swirl amount: more dynamic, with layered sine waves for richer motion
float swirlAmount = time * 1.4
                  + 0.25 * sin(time * 1.2 + clampedRadius * 4.0)
                  + 0.12 * sin(time * 2.3 + clampedRadius * 7.0);

// Swirl angle: use clamped radius for swirl, so vertices outside limit don't "wrap"
float angle = atan(aPos.y, aPos.x) + swirlAmount * (clampedRadius / swirlRadiusLimit) * swirlStrength;

// Ripple for organic look, with subtle multi-frequency detail
float ripple = 0.06 * sin(12.0 * clampedRadius - time * 2.0)
             + 0.02 * sin(24.0 * clampedRadius + time * 1.5);
float modRadius = clampedRadius + ripple * swirlStrength;

// Swirled position in XY
vec2 swirlPos = modRadius * vec2(cos(angle), sin(angle));

// If outside swirl limit, keep original direction, offset by swirled position at the limit
if (radius > swirlRadiusLimit) {
  float baseAngle = atan(aPos.y, aPos.x) + swirlAmount;
  vec2 baseSwirl = swirlRadiusLimit * vec2(cos(baseAngle), sin(baseAngle));
  vec2 dir = normalize(aPos.xy);
  swirlPos = baseSwirl + (radius - swirlRadiusLimit) * dir;
}

// Compose final swirled position
vec3 swirledPos = vec3(swirlPos, aPos.z);

// Transform to world space
vec4 worldPosition = modelMatrix * vec4(swirledPos, 1.0);
fragPosition = worldPosition.xyz;

// Swirl the normal (rotate in XY by swirl angle, only within swirl radius)
float normalAngle = swirlAmount * (clampedRadius / swirlRadiusLimit) * swirlStrength;
mat2 rot = mat2(cos(normalAngle), -sin(normalAngle), sin(normalAngle), cos(normalAngle));
vec3 swirledNormal = aNormal;
swirledNormal.xy = rot * aNormal.xy;
vec3 worldNormal = normalize(mat3(modelMatrix) * swirledNormal);
fragNormal = worldNormal;

// Swirl tangent and bitangent for correct TBN
vec3 swirledTangent = aTangent;
swirledTangent.xy = rot * aTangent.xy;
vec3 swirledBitangent = aBitangent;
swirledBitangent.xy = rot * aBitangent.xy;
vec3 T = normalize(mat3(modelMatrix) * swirledTangent);
vec3 B = normalize(mat3(modelMatrix) * swirledBitangent);
TBN = mat3(T, B, worldNormal);

// Swirl texture coordinates for a more visible, animated effect
fragTexCoord = aTexCoord
  + 0.015 * swirlStrength * vec2(sin(time * 1.1 + aTexCoord.y * 12.0), cos(time * 1.3 + aTexCoord.x * 12.0))
  + 0.008 * swirlStrength * vec2(sin(time * 2.0 + aTexCoord.x * 24.0), cos(time * 2.2 + aTexCoord.y * 24.0));

// Output final position
gl_Position = projMatrix * viewMatrix * worldPosition;
}
