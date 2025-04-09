#pragma once
#include "Light.h"
#include "DirectionLight.h"

struct pointLight {
	glm::vec3 m_pos;
	glm::vec3 m_col;
	glm::vec3 m_attenuation; // x = constant, y = linear, z = quadratic

};

class PointLight : public Light
{
public:
	PointLight();
	~PointLight();
	void Tick(float _dt);
	void Load(ifstream& _file)override;
	void SetRenderValues(unsigned int _prog)override;

	

	void SetPointLights(unsigned int shaderProgram, const std::vector<pointLight>& lights);
	std::vector<pointLight> lights;

	vec3 m_direction;
	vec3 m_attenuation;
	float numLights;

protected:
	
	float m_radius;
	float m_constant;
	float m_linear;
	float m_quadratic;
	
};