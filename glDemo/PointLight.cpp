#include "PointLight.h"

PointLight::PointLight()
{
	m_type = "POINT";
	m_pos = vec3(0.0, 0.0, 0.0);
	m_direction = vec3(0.0, 1.0, 0.0);
}
PointLight::~PointLight()
{

}
void PointLight::Load(ifstream& _file)
{
	Light::Load(_file);
}



