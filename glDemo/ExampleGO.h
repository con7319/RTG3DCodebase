#pragma once

#include "GameObject.h"
#include "AIModel.h"
#include "array"
class AIModel;
class Texture;

//replicate the examples from the main.cpp
//each Game Object has a Mesh, a texture and a shader
//which it uses to render itself
class ExampleGO :
	public GameObject
{
public:
	ExampleGO();
	~ExampleGO();

	//load me from the file
	virtual void Load(ifstream& _file);

	//update _window allows for Keyboard access
	virtual void Tick(float _dt);

	//render this object
	virtual void PreRender();
	virtual void Render();

	virtual void Init(Scene* _scene);

	//added getter and setter for model
	AIModel* GetModel() const { return m_model; }
	void SetModel(AIModel* model) { m_model = model; }

	virtual void SetPos(glm::vec3 _pos) { m_pos = _pos; }
	virtual glm::vec3 GetPos() const { return m_pos; }
	
	void addTexture(GLuint _textureID);
	void addNormalMap(GLuint _normalMapID);
	void setTexScale(unsigned int _prog, float _scale);
	AIModel* m_model;

protected:

	string m_ShaderName, m_TexName, m_ModelName, m_normalName;
	bool normalLoaded;
	float m_NoTex;
	float m_NoMap;
	GLuint m_texture;
	std::vector<std::string> m_normNames;
	std::vector<std::string> m_texNames;
	std::vector<GLuint> m_texList;
	std::vector<GLuint> m_normList;
	
	Scene* m_scene;
	GLuint m_NormalMap;
};

