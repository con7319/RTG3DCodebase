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

float noise(vec3 p) {
return fract(sin(dot(p, vec3(12.9898, 78.233, 45.164))) * 43758.5453);
}

void main()
{
 float waveAmplitude = 0.15 + sin(time * 0.5) * 0.05;
 float waveFrequency = 12.0 + cos(time * 0.3) * 2.0;
 float noiseStrength = 0.07 + sin(time * 0.8) * 0.02;
 float noiseFrequency = 4.0 + cos(time * 0.6) * 1.0;

 vec3 worldNormal = normalize((modelMatrix * vec4(aNormal, 0.0)).xyz);
 vec3 worldPos = (modelMatrix * vec4(aPos, 1.0)).xyz;

 // Wave effect with a twist
 float wave = sin(worldPos.x * waveFrequency + time) * waveAmplitude;
 wave += cos(worldPos.y * waveFrequency * 0.5 + time * 1.5) * waveAmplitude * 0.5;

 // Noise effect with extra chaos
 float noiseValue = noise(vec3(worldPos.xz * noiseFrequency, time)) * 2.0 - 1.0;
 float noiseDisplacement = noiseValue * noiseStrength;
 noiseDisplacement += sin(noiseValue * 3.14) * 0.02;

 // Swirl effect
 float swirlStrength = 0.1 + sin(time * 0.4) * 0.05;
 float swirlRadius = 2.0 + cos(time * 0.2) * 0.5;
 vec2 swirlOffset = vec2(sin(worldPos.y * swirlRadius + time), cos(worldPos.x * swirlRadius + time)) * swirlStrength;

 // Combine wave, noise, and swirl
 float totalDisplacement = wave + noiseDisplacement;
 totalDisplacement *= 1.0 + 0.1 * sin(worldPos.z * 5.0 + time * 2.0);

 vec3 worldDisplacedPos = worldPos + worldNormal * totalDisplacement;
 worldDisplacedPos.xy += swirlOffset;

 fragTexCoord = aTexCoord;
 fragPosition = worldDisplacedPos;
 fragNormal = normalize((transpose(inverse(mat3(modelMatrix))) * aNormal));

 gl_Position = projMatrix * viewMatrix * vec4(worldDisplacedPos, 1.0);
}