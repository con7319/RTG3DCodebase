#pragma once
#include <string>
#include "GameObject.h"

using namespace std;

//base class for Models that can be owned by GameObjects so they can be rendered
//current empty as this a interface/strawman to allow them to be put in the same data structure
class Model :public GameObject
{
public:
	Model();
	virtual ~Model();

	virtual void Load(ifstream& _file);
	virtual void Render() {};

	string GetName() { return m_name; }

protected:
	string m_name;
	string m_type;

	GLuint				m_numFaces = 0;
	GLuint				m_vao = 0;

	GLuint				m_vertexBuffer;
	GLuint				m_colourBuffer;
	GLuint				m_indexBuffer;
};