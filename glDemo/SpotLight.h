#pragma once
#include "Light.h"


struct spotLight {
	glm::vec3 m_pos;
	glm::vec3 m_col;
	glm::vec3 m_attenuation; // x = constant, y = linear, z = quadratic
	glm::vec3 m_direction;
	float m_cutoff;
	float m_outerCutoff;
};

class SpotLight : public Light
{
public:
	SpotLight();
	~SpotLight();
	void Tick(float _dt);
	void Load(ifstream& _file)override;
	void SetRenderValues(unsigned int _prog)override;
	void SetSpotLights(unsigned int shaderProgram, const std::vector<spotLight>& lights);
	std::vector<spotLight> lights;
	vec3 m_direction;
	vec3 m_attenuation;
	float numLights;
};