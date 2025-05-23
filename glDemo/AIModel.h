#pragma once
#include "Model.h"
class AIMesh;

//Uses ASSIMP to load/render in an obj model

class AIModel :
	public Model
{
public:
	AIModel();
	virtual ~AIModel();

	void Load(ifstream& _file);
	virtual void Render();
	void addNormalMap(GLuint _normalMapID);
	void addTexture(GLuint _textureID);
	void setupTextures(unsigned int _prog);
	void setTexture(GLuint textureID, int unit);
	void setTexScale(unsigned int _prog, float _scale);
	void setBlendScale(unsigned int _prog, float _scale);

	void setSecondaryTexture(GLuint textureID);

	AIMesh* m_AImesh;
protected:
	
};

