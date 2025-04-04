#pragma once
#include "Light.h"
#include "DirectionLight.h"


class PointLight : public Light
{
public:
	PointLight();
	~PointLight();
	void Load(ifstream& _file)override;
	

protected:
	vec3 m_pos;
	vec3 m_direction;
	
	float m_radius;
	float m_constant;
	float m_linear;
	float m_quadratic;
	
};