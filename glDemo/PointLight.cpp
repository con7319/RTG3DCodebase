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
	StringHelp::Float3(_file, "ATT", m_attenuation.x, m_attenuation.y, m_attenuation.z);
}
void PointLight::SetRenderValues(unsigned int _prog)
{
	GLint loc;
	string posString = m_type + "Pos";
	string colString = m_type + "Col";
	string ambString = m_type + "Amb";
	string attString = m_type + "Att";

	if (Helper::SetUniformLocation(_prog, posString.c_str(), &loc))
		glUniform3fv(loc, 1, glm::value_ptr(GetPos()));

	if (Helper::SetUniformLocation(_prog, colString.c_str(), &loc))
		glUniform3fv(loc, 1, glm::value_ptr(GetCol()));

	if (Helper::SetUniformLocation(_prog, ambString.c_str(), &loc))
		glUniform3fv(loc, 1, glm::value_ptr(GetAmb()));
	
	if (Helper::SetUniformLocation(_prog, attString.c_str(), &loc))
		glUniform3fv(loc, 1, glm::value_ptr(m_attenuation));

}




