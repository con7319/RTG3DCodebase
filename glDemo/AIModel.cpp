#include "AIModel.h"
#include "stringHelp.h"
#include "AIMesh.h"

AIModel::AIModel()
{
	m_type = "AI";
	m_AImesh = nullptr;
}

AIModel::~AIModel()
{
	delete m_AImesh;
}

void AIModel::Load(ifstream& _file)
{
	Model::Load(_file);
	string fileName;
	StringHelp::String(_file, "FILE", fileName);

	m_AImesh = new AIMesh(fileName);
}

void AIModel::Render()
{
	m_AImesh->render();
}
void AIModel::addNormalMap(GLuint _normalMapID)
{
	m_AImesh->addNormalMap(_normalMapID);
}
void AIModel::addTexture(GLuint _textureID)
{
	if (m_AImesh != nullptr)
	{
		m_AImesh->addTexture(_textureID);
	}
}

void AIModel::setupTextures(unsigned int _prog)
{
	if (m_AImesh != nullptr)
	{
		m_AImesh->setupTextures(_prog);
	}
}
void AIModel::setTexScale(unsigned int _prog, float _scale) 
{
	if (m_AImesh != nullptr)
	{
		m_AImesh->setTexScale(_prog, _scale);
	}


}
void AIModel::setBlendScale(unsigned int _prog, float _scale)
{
	if (m_AImesh != nullptr)
	{
		m_AImesh->setBlendFactor(_prog, _scale);
	}
}
void AIModel::setSecondaryTexture(GLuint textureID)
{
	if (m_AImesh != nullptr)
	{
		m_AImesh->addTexture(textureID);
	}
}
void AIModel::setTexture(GLuint textureID, int unit) {
	if (m_AImesh) {
		m_AImesh->setTexture(textureID, unit);
	}
}
