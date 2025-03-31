#include "ExampleGO.h"
#include "Scene.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "AIModel.h"
#include "stringHelp.h"


ExampleGO::ExampleGO()
{
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
   StringHelp::String(_file, "NORMALMAP", m_normalName);

   if (m_normalName == "NULL") {
	   normalLoaded = false;
   }
   else {
	   normalLoaded = true;
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

	glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	//TODO: this does sort of replicate stuff in the AIMesh class, could we make them more compatible.

	//TODO: NORMAL MAPS!
	if (normalLoaded) {

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_NormalMap);
	}
	
}

void ExampleGO::Render()
{
	
	m_model->Render();
}

void ExampleGO::Init(Scene* _scene)
{
	m_scene = _scene;
	m_ShaderProg = _scene->GetShader(m_ShaderName)->GetProg();
	for (string name : m_texNames)
	{
		m_texList.push_back (_scene->GetTexture(name)->GetTexID());
	}
	m_model = _scene->GetModel(m_ModelName);

	if (normalLoaded) 
	{
		m_NormalMap = _scene->GetTexture(m_normalName)->GetTexID();
	}
	
}


