#include "core.h"
#include "DirectionLight.h"
#include "helper.h"
#include "stringHelp.h"

DirectionLight::DirectionLight()
{
	m_type = "DIRECTION";
	m_pos = vec3(0.0, 0.0, 0.0);
	m_direction = vec3(0.0, 1.0, 0.0);
}

DirectionLight::~DirectionLight()
{
}

void DirectionLight::Load(ifstream& _file)
{

	StringHelp::String(_file, "NAME", m_name);
	StringHelp::Float(_file, "NUMLIGHTS", numLights);
	for (int i = 0; i < numLights; ++i)
	{
		dirLight light;
		StringHelp::Float3(_file, "POS", light.m_pos.x, light.m_pos.y, light.m_pos.z);
		StringHelp::Float3(_file, "COL", light.m_col.x, light.m_col.y, light.m_col.z);
		StringHelp::Float3(_file, "AMB", light.m_amb.x, light.m_amb.y, light.m_amb.z);
		StringHelp::Float3(_file, "DIR", light.m_dir.x, light.m_dir.y, light.m_dir.z);
		lights.push_back(light);

	}


	//Light::Load(_file);
	//StringHelp::Float3(_file, "DIRECTION", m_direction.x, m_direction.y, m_direction.z);
}

void DirectionLight::SetRenderValues(unsigned int _prog)
{
	//still need to tell the shader about the basic light data
	Light::SetRenderValues(_prog);

	SetDirLights(_prog, lights);
	//GLint loc;
	//string dirString = m_name + "Dir";//only thing I add is a direction

	//if (Helper::SetUniformLocation(_prog, dirString.c_str(), &loc))
	//	glUniform3fv(loc, 1, glm::value_ptr(m_direction));
}

void DirectionLight::SetDirLights(unsigned int shaderProgram, const std::vector<dirLight>& lights) {
	glUseProgram(shaderProgram);

	// Pass the number of lights
	GLint numLightsLoc = glGetUniformLocation(shaderProgram, "numDirLights");
	glUniform1i(numLightsLoc, static_cast<GLint>(lights.size()));

	// Pass each light's data
	for (size_t i = 0; i < lights.size(); ++i) {
		std::string baseName = "dirLights[" + std::to_string(i) + "].";

		GLint posLoc = glGetUniformLocation(shaderProgram, (baseName + "d_pos").c_str());
		GLint colLoc = glGetUniformLocation(shaderProgram, (baseName + "d_col").c_str());
		GLint ambLoc = glGetUniformLocation(shaderProgram, (baseName + "d_amb").c_str());
		GLint dirLoc = glGetUniformLocation(shaderProgram, (baseName + "d_dir").c_str());

		glUniform3fv(posLoc, 1, &lights[i].m_pos[0]);
		glUniform3fv(colLoc, 1, &lights[i].m_col[0]);
		glUniform3fv(ambLoc, 1, &lights[i].m_amb[0]);
		glUniform3fv(dirLoc, 1, &lights[i].m_dir[0]);
	}
}
