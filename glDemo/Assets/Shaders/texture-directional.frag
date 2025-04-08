#version 450 core

// Diffuse texture - directional light

// Texture sampler (for diffuse surface colour)
layout(binding = 0) uniform sampler2D texture;

// Directional light model
uniform vec3 DIRDir;
uniform vec3 DIRCol;
uniform vec3 DIRAmb;

uniform vec3 POINTPos;
uniform vec3 POINTCol;
uniform vec3 POINTAmb;
uniform vec3 POINTAtt;


in SimplePacket {
	
	vec3 surfaceWorldPos;
	vec3 surfaceNormal;
	vec2 texCoord;

} inputFragment;


layout (location=0) out vec4 fragColour;

void main(void) {

	// calculate lambertian (l)
	vec3 N = normalize(inputFragment.surfaceNormal);
	float l = dot(N, DIRDir);

	// Calculate diffuse brightness / colour for fragment
	vec4 surfaceColour = texture2D(texture, inputFragment.texCoord);
	vec3 diffuseColour = surfaceColour.rgb * DIRCol * l;

	vec3 surfaceToLightVec = POINTPos - inputFragment.surfaceWorldPos;
	vec3 surfaceToLightNormalised = normalize(surfaceToLightVec);
	vec3 PN = normalize(inputFragment.surfaceNormal);
	float Pl = dot(PN, surfaceToLightNormalised);

	float d = length(surfaceToLightVec);
	float kc = POINTAtt.x;
	float kl = POINTAtt.y;
	float kq = POINTAtt.z;
	float a = 1.0 / ( kc + (kl * d) + (kq * d * d) );

	
	vec3 PdiffuseColour = surfaceColour.rgb * POINTCol * Pl * a;
	vec3 finalColour = DIRAmb + diffuseColour * PdiffuseColour;
	fragColour = vec4(finalColour, surfaceColour.a);


	//fragColour = vec4(DIRAmb,1.0)+vec4(diffuseColour, 1.0);
	//fragColour = vec4(vec3(l, l, l), 1.0);
}
