#include "ExampleGO.h"
#include "Scene.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "AIModel.h"
#include "stringHelp.h"


ExampleGO::ExampleGO()
{
	normalLoaded = false;
}

ExampleGO::~ExampleGO()
{
}

void ExampleGO::Load(ifstream& _file)  
{  
   GameObject::Load(_file);  
   StringHelp::String(_file, "MODEL", m_ModelName);  
   StringHelp::Float(_file, "NOTEX", m_NoTex);  
   for (int i = 0; i < m_NoTex; ++i)
   {
		std::string texName;
		StringHelp::String(_file, "TEXTURE", texName);
		m_texNames.push_back(texName);

   }
   StringHelp::String(_file, "SHADER", m_ShaderName); 
   

   if (m_ShaderName == "NORM") {
	   
	   StringHelp::Float(_file, "NOMAPS", m_NoMap);
	   for (int i = 0; i < m_NoMap; ++i)
	   {
		   std::string NormName;
		   StringHelp::String(_file, "NORMALMAP", NormName);
		   m_normNames.push_back(NormName);
		   
	   }
	   normalLoaded = true;
   }
   if (m_ShaderName == "FRACTAL") {

	   StringHelp::Float(_file, "NOMAPS", m_NoMap);
	   for (int i = 0; i < m_NoMap; ++i)
	   {
		   std::string NormName;
		   StringHelp::String(_file, "NORMALMAP", NormName);
		   m_normNames.push_back(NormName);

	   }
	   normalLoaded = true;
	   FractalShader = true;
   }
   else {
	   
   }
}

void ExampleGO::Tick(float _dt)
{
	GameObject::Tick(_dt);
}

void ExampleGO::PreRender()
{
	GameObject::PreRender();

	//only thing I need to do is tell the shader about my texture
	if (normalLoaded) {

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_NormalMap);

	}

	if (m_RP == RP_TRANSPARENT)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glDisable(GL_BLEND);
	}

	m_model->setupTextures(m_ShaderProg);
	/*glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glUniform1i(glGetUniformLocation(m_ShaderProg, "texture"), 0);*/

	//TODO: this does sort of replicate stuff in the AIMesh class, could we make them more compatible.

	//TODO: NORMAL MAPS!
	

	//glDisable(GL_TEXTURE_2D);
}

void ExampleGO::Render()
{
	
	m_model->Render();
}

void ExampleGO::Init(Scene* _scene)
{
	m_scene = _scene;
	m_ShaderProg = _scene->GetShader(m_ShaderName)->GetProg();
	m_model = _scene->GetModel(m_ModelName);

	for (string name : m_texNames)
	{
		addTexture(_scene->GetTexture(name)->GetTexID());
		m_texList.push_back (_scene->GetTexture(name)->GetTexID());
	}
	
	

	if (normalLoaded) 
	{
		for (string normn : m_normNames)
		{
			addNormalMap(_scene->GetTexture(normn)->GetTexID());
			m_normList.push_back(_scene->GetTexture(normn)->GetTexID());
			std::cout << "Normal map loaded: " << normn << std::endl;

		}

	}
	if (FractalShader) {
		m_model->setBlendScale(m_ShaderProg, 0.5f);
		m_model->setSecondaryTexture(m_texList[1]);
		m_model->setTexture(m_texList[1], 2);
	}
	
	
}
void ExampleGO::addTexture(GLuint _textureID)  
{  
   m_model->addTexture(_textureID);  
}
void ExampleGO::addNormalMap(GLuint _normalMapID)
{
	m_model->addNormalMap(_normalMapID);
	
}
void ExampleGO::setTexScale(unsigned int _prog, float _scale)
{
	m_model->setTexScale(_prog, _scale);
}
