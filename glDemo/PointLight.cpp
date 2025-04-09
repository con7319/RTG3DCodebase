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
void PointLight::Tick(float _dt)  
{  
	bool movePoint = false;
	if (movePoint) {

		static float time = 0.0f;
		time += _dt;

		float amplitude = 5.0f; // Amplitude of oscillation
		float frequency = 3.0f; // Frequency of oscillation

		for (int i = 0; i < lights.size(); ++i)
		{
			lights[i].m_pos.z = amplitude * sin(frequency * time);

		}
	}
   
 
   
}
void PointLight::SetPointLights(unsigned int shaderProgram, const std::vector<pointLight>& lights) {
	glUseProgram(shaderProgram);

	// Pass the number of lights
	GLint numLightsLoc = glGetUniformLocation(shaderProgram, "numPointLights");
	glUniform1i(numLightsLoc, static_cast<GLint>(lights.size()));

	// Pass each light's data
	for (size_t i = 0; i < lights.size(); ++i) {
		std::string baseName = "pointLights[" + std::to_string(i) + "].";

		GLint posLoc = glGetUniformLocation(shaderProgram, (baseName + "p_pos").c_str());
		GLint colLoc = glGetUniformLocation(shaderProgram, (baseName + "p_col").c_str());
		GLint attLoc = glGetUniformLocation(shaderProgram, (baseName + "p_attenuation").c_str());

		glUniform3fv(posLoc, 1, &lights[i].m_pos[0]);
		glUniform3fv(colLoc, 1, &lights[i].m_col[0]);
		glUniform3fv(attLoc, 1, &lights[i].m_attenuation[0]);
	}
}

void PointLight::Load(ifstream& _file)
{
	StringHelp::String(_file, "NAME", m_name);
	StringHelp::Float(_file, "NUMLIGHTS", numLights);
	for (int i = 0; i < numLights; ++i) 
	{
		pointLight light;
		StringHelp::Float3(_file, "POS", light.m_pos.x, light.m_pos.y, light.m_pos.z);
		StringHelp::Float3(_file, "COL", light.m_col.x, light.m_col.y, light.m_col.z);
		StringHelp::Float3(_file, "ATT", light.m_attenuation.x, light.m_attenuation.y, light.m_attenuation.z);
		lights.push_back(light);
	
	}
	//StringHelp::Float3(_file, "POS", m_pos.x, m_pos.y, m_pos.z);
	//StringHelp::Float3(_file, "COL", m_col.x, m_col.y, m_col.z);
	//StringHelp::Float3(_file, "ATT", m_attenuation.x, m_attenuation.y, m_attenuation.z);

	//// Create a PointLight object instead of pointLight struct
	//pointLight light;
	//light.m_pos = m_pos;
	//light.m_col = m_col;
	//light.m_attenuation = m_attenuation;

	//// Push the object into the vector
	//lights.push_back(light);
}
void PointLight::SetRenderValues(unsigned int _prog)
{
	/*GLint loc;
	string posString = m_type + "Pos";
	string colString = m_type + "Col";
	string attString = m_type + "Att";

	if (Helper::SetUniformLocation(_prog, posString.c_str(), &loc))
		glUniform3fv(loc, 1, glm::value_ptr(GetPos()));

	if (Helper::SetUniformLocation(_prog, colString.c_str(), &loc))
		glUniform3fv(loc, 1, glm::value_ptr(GetCol()));
	
	if (Helper::SetUniformLocation(_prog, attString.c_str(), &loc))
		glUniform3fv(loc, 1, glm::value_ptr(m_attenuation));*/

	SetPointLights(_prog, lights);

}




