#pragma once
#include "Light.h"

//a simple directional light
//essentially only has one difference a direction variable
struct dirLight {
	glm::vec3 m_pos;
	glm::vec3 m_col;
	glm::vec3 m_amb;
	glm::vec3 m_dir;

};
class DirectionLight :
	public Light
{
public:
	DirectionLight();
	~DirectionLight();

	//load from manifest
	virtual void Load(ifstream& _file);

	//set render values
	virtual void SetRenderValues(unsigned int _prog);

	std::vector<dirLight> lights;

	void SetDirLights(unsigned int shaderProgram, const std::vector<dirLight>& lights);
	//TODO: We don't have our own tick
	// a nice feature would be a day / night cycle effect 

protected:
	float numLights;
	vec3 m_direction;

};

