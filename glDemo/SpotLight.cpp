#include "SpotLight.h"

SpotLight::SpotLight()
{
m_type = "SPOT";
m_pos = vec3(0.0, 0.0, 0.0);
m_direction = vec3(0.0, 1.0, 0.0);
m_lookAt = vec3(0.0, 0.0, 0.0); // Initialize m_lookAt
}
SpotLight::~SpotLight()
{
}

void SpotLight::Tick(float _dt)  
{  
 bool movePoint = false;  
 if (movePoint) {  
     static float time = 0.0f;  
     time += _dt;  
     float amplitude = 5.0f; // Amplitude of oscillation  
     float frequency = 3.0f; // Frequency of oscillation  
     for (int i = 0; i < lights.size(); ++i)  
     {  
         // Update position with oscillation  
         lights[i].m_pos.z = amplitude * sin(frequency * time);  

         // Update direction to rotate over time  
         float rotationSpeed = 4.0f; // Speed of rotation  
         float angle = rotationSpeed * time;  
         lights[i].m_direction.x = sin(angle);  
         lights[i].m_direction.y = cos(angle);  
         lights[i].m_direction.z = 0.0f; // Keep direction in the XY plane  
     }  
 }  

 // Always look at m_lookAt
 for (int i = 0; i < lights.size(); ++i)  
 {  
     lights[i].m_direction = glm::normalize(lights[i].m_lookAt - lights[i].m_pos);  
 }  

    
}
void SpotLight::Load(ifstream& _file) 
{
StringHelp::String(_file, "NAME", m_name);
StringHelp::Float(_file, "NUMLIGHTS", numLights);
for (int i = 0; i < numLights; ++i)
{
	spotLight light;
	StringHelp::Float3(_file, "POS", light.m_pos.x, light.m_pos.y, light.m_pos.z);
	StringHelp::Float3(_file, "COL", light.m_col.x, light.m_col.y, light.m_col.z);
	StringHelp::Float3(_file, "ATT", light.m_attenuation.x, light.m_attenuation.y, light.m_attenuation.z);
	StringHelp::Float3(_file, "DIR", light.m_direction.x, light.m_direction.y, light.m_direction.z);
	StringHelp::Float(_file, "CUTOFF", light.m_cutoff);
	StringHelp::Float(_file, "OCUTOFF", light.m_outerCutoff);
	lights.push_back(light);
}

}
void SpotLight::SetSpotLights(unsigned int shaderProgram, const std::vector<spotLight>& lights) {
glUseProgram(shaderProgram);
// Pass the number of lights
GLint numLightsLoc = glGetUniformLocation(shaderProgram, "numSpotLights");
glUniform1i(numLightsLoc, static_cast<GLint>(lights.size()));
// Pass each light's data
for (size_t i = 0; i < lights.size(); ++i) {
	std::string baseName = "spotLights[" + std::to_string(i) + "].";
	GLint posLoc = glGetUniformLocation(shaderProgram, (baseName + "s_pos").c_str());
	GLint colLoc = glGetUniformLocation(shaderProgram, (baseName + "s_col").c_str());
	GLint attLoc = glGetUniformLocation(shaderProgram, (baseName + "s_att").c_str());
	GLint dirLoc = glGetUniformLocation(shaderProgram, (baseName + "s_dir").c_str());
	GLint cutoffLoc = glGetUniformLocation(shaderProgram, (baseName + "s_cutoff").c_str());
	GLint outerCutoffLoc = glGetUniformLocation(shaderProgram, (baseName + "s_oCutoff").c_str());
	glUniform3fv(posLoc, 1, &lights[i].m_pos[0]);
	glUniform3fv(colLoc, 1, &lights[i].m_col[0]);
	glUniform3fv(attLoc, 1, &lights[i].m_attenuation[0]);
	glUniform3fv(dirLoc, 1, &lights[i].m_direction[0]);
	glUniform1f(cutoffLoc, glm::cos(glm::radians(lights[i].m_cutoff)));
	glUniform1f(outerCutoffLoc, glm::cos(glm::radians(lights[i].m_outerCutoff)));
}
}

void SpotLight::SetRenderValues(unsigned int _prog)
{
//still need to tell the shader about the basic light data
SetSpotLights(_prog, lights);
}

void SpotLight::LookAt(const vec3& target)  
{  
	
	for (int i = 0; i < lights.size(); ++i)
	{
		lights[i].m_lookAt = target;
	}
	
}