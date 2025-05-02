#include "Scene.h"
#include "GameObject.h"
#include "CameraFactory.h"
#include "Camera.h"
#include "LightFactory.h"
#include "Light.h"
#include "ModelFactory.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "GameObjectFactory.h"
#include <assert.h>
#include "ArcballCamera.h"
#include "FPcam.h"
#include "OrthoCam.h"
#include "AIModel.h"
#include "helper.h"
#include "LevelGen.h"


Scene::Scene()
{
}

Scene::~Scene()
{
	//TODO: We are being really naughty and not deleting everything as we finish
	//what shoudl really go here and in similar places throughout the code base?
}

//tick all my Game Objects, lights and cameras
void Scene::Update(float _dt, float _width, float _height)
{
	//update all lights
	for (list<Light*>::iterator it = m_Lights.begin(); it != m_Lights.end(); it++)
	{
		(*it)->Tick(_dt);
	}

	//update all cameras
	for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); it++)
	{
		(*it)->Tick(_dt,_width,_height);
	}

	//update all GameObjects
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		(*it)->Tick(_dt);
	}
}

void Scene::AddGameObject(GameObject* _new)
{
	m_GameObjects.push_back(_new);
}

//I want THAT Game Object by name
GameObject* Scene::GetGameObject(string _GOName)
{
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		if ((*it)->GetName() == _GOName)
		{
			return (*it);
		}
	}
	printf("Unknown Game Object NAME : %s \n", _GOName.c_str());
	assert(0);
	return nullptr;
}

Camera* Scene::GetCamera(string _camName)
{
	for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); it++)
	{
		if ((*it)->GetName() == _camName)
		{
			return (*it);
		}
	}
	
	printf("Unknown Camera NAME : %s \n", _camName.c_str());
	assert(0);
	return nullptr;
}


Light* Scene::GetLight(string _lightName)
{
	for (list<Light*>::iterator it = m_Lights.begin(); it != m_Lights.end(); it++)
	{
		if ((*it)->GetName() == _lightName)
		{
			return (*it);
		}
	}
	printf("Unknown Light NAME : %s \n", _lightName.c_str());
	assert(0);
	return nullptr;
}

Texture* Scene::GetTexture(string _texName)
{
	for (list<Texture*>::iterator it = m_Textures.begin(); it != m_Textures.end(); it++)
	{
		if ((*it)->GetName() == _texName)
		{
			return (*it);
		}
	}
	printf("Unknown Texture NAME : %s \n", _texName.c_str());
	assert(0);
	return nullptr;
}

AIModel* Scene::GetModel(string _modelName)
{
	for (list<AIModel*>::iterator it = m_Models.begin(); it != m_Models.end(); it++)
	{
		if ((*it)->GetName() == _modelName)
		{
			return (*it);
		}
	}
	printf("Unknown Model NAME : %s \n", _modelName.c_str());
	assert(0);
	return nullptr;
}

Shader* Scene::GetShader(string _shaderName)
{
	for (list<Shader*>::iterator it = m_Shaders.begin(); it != m_Shaders.end(); it++)
	{
		if ((*it)->GetName() == _shaderName)
		{
			return (*it);
		}
	}
	printf("Unknown Shader NAME : %s \n", _shaderName.c_str());
	assert(0);
	return nullptr;
}


//Render Everything
void Scene::Render()
{
	//TODO: Set up for the Opaque Render Pass will go here
	//check out the example stuff back in main.cpp to see what needs setting up here
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		if ((*it)->GetRP() & RP_OPAQUE)// TODO: note the bit-wise operation. Why?
		{
			//set shader program using
			GLuint SP = (*it)->GetShaderProg();
			glUseProgram(SP);

			//set up for uniform shader values for current camera
			m_useCamera->SetRenderValues(SP);

			//loop through setting up uniform shader values for anything else
			SetShaderUniforms(SP);

			if (m_useCamera && m_useCamera->GetType() == "ARCBALL") {
				ArcballCamera* arcballCam = dynamic_cast<ArcballCamera*>(m_useCamera);
				if (arcballCam) {
					glm::mat4 projectionMatrix = arcballCam->projectionTransform();
					glm::mat4 viewMatrix = arcballCam->viewTransform();

					GLint pLocation;
					Helper::SetUniformLocation(SP, "viewMatrix", &pLocation);
					glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&viewMatrix);

					Helper::SetUniformLocation(SP, "projMatrix", &pLocation);
					glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&projectionMatrix);
				}
			}
			if (m_useCamera && m_useCamera->GetType() == "FIRST") {
				FPcam* firstPersonCam = dynamic_cast<FPcam*>(m_useCamera);
				if (firstPersonCam) {
					glm::mat4 projectionMatrix = firstPersonCam->projectionTransform();
					glm::mat4 viewMatrix = firstPersonCam->viewTransform();

					GLint pLocation;
					Helper::SetUniformLocation(SP, "viewMatrix", &pLocation);
					glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&viewMatrix);

					Helper::SetUniformLocation(SP, "projMatrix", &pLocation);
					glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&projectionMatrix);
				}
			}
			if (m_useCamera && m_useCamera->GetType() == "ORTHO") {
				OrthoCam* orthoCam = dynamic_cast<OrthoCam*>(m_useCamera);
				if (orthoCam) {
					glm::mat4 projectionMatrix = orthoCam->projectionTransform();
					glm::mat4 viewMatrix = orthoCam->viewTransform();

					GLint pLocation;
					Helper::SetUniformLocation(SP, "viewMatrix", &pLocation);
					glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&viewMatrix);

					Helper::SetUniformLocation(SP, "projMatrix", &pLocation);
					glUniformMatrix4fv(pLocation, 1, GL_FALSE, (GLfloat*)&projectionMatrix);
				}
			}

			
			//set any uniform shader values for the actual model
			(*it)->PreRender();

			//actually render the GameObject
			(*it)->Render();
		}

		//TODO: now do the same for RP_TRANSPARENT here

		if ((*it)->GetRP() & RP_TRANSPARENT)
		{
			//Enable blending
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			// Disable depth writes
			glDepthMask(GL_FALSE);
			// Set shader program
			GLuint SP = (*it)->GetShaderProg();
			glUseProgram(SP);
			// Set up uniform shader values for the current camera
			m_useCamera->SetRenderValues(SP);
			// Set up uniform shader values for lights and other global settings
			SetShaderUniforms(SP);
			// Set any uniform shader values for the actual model
			(*it)->PreRender();
			// Render the GameObject
			(*it)->Render();
			// Re-enable depth writes
			glDepthMask(GL_TRUE);
			// Disable blending
			glDisable(GL_BLEND);
		}
	}

	


}

void Scene::SetShaderUniforms(GLuint _shaderprog)
{
	//everything needs to know about all the lights
	for (list<Light*>::iterator it = m_Lights.begin(); it != m_Lights.end(); it++)
	{
		if ((*it)->GetType() == "POINT")
		{
			//set up shader values for this light
			(*it)->SetRenderValues(_shaderprog);
		}
		else if ((*it)->GetType() == "DIRECTIONAL")
		{
			//set up shader values for this light
			(*it)->SetRenderValues(_shaderprog);
		}
		else if ((*it)->GetType() == "SPOT")
		{
			//set up shader values for this light
			(*it)->SetRenderValues(_shaderprog);
		}
		(*it)->SetRenderValues(_shaderprog);
	}

}

void Scene::Load(ifstream& _file)
{
	string dummy;
	count = 0;

	while (!_file.eof()) //end check
	{
		_file >> dummy >> count;
		_file.ignore(256, '\n');


		if (dummy == "CAMERAS")
		{
			//load Cameras
			m_numCameras = count;
			//_file >> dummy >> m_numCameras; _file.ignore(256, '\n');

			std::cout << "CAMERAS : " << m_numCameras << endl;
			for (int i = 0; i < m_numCameras; i++)
			{
				//skip {
				_file.ignore(256, '\n');
				std::cout << "{\n";

				string type;
				_file >> dummy >> type; _file.ignore(256, '\n');
				Camera* newCam = CameraFactory::makeNewCam(type);
				newCam->Load(_file);

				m_Cameras.push_back(newCam);

				//skip }
				_file.ignore(256, '\n');
				std::cout << "}\n";
			}

			std::cout << endl << endl;

		}

		//_file >> dummy >> count; _file.ignore(256, '\n');
		//load Lights
		else if (dummy == "LIGHTS")
		{
			m_numLights = count;

			//_file >> dummy >> m_numLights; _file.ignore(256, '\n');
			std::cout << "LIGHTS : " << m_numLights << endl;
			for (int i = 0; i < m_numLights; i++)
			{
				//skip {
				_file.ignore(256, '\n');
				std::cout << "{\n";

				string type;
				_file >> dummy >> type; _file.ignore(256, '\n');
				Light* newLight = LightFactory::makeNewLight(type);
				newLight->Load(_file);
				

				m_Lights.push_back(newLight);

				//skip }
				_file.ignore(256, '\n');
				std::cout << "}\n";
			}
			std::cout << endl << endl;
		}



		//_file >> dummy >> count; _file.ignore(256, '\n');

		else if (dummy == "MODELS")
		{

			//load Models
			//_file >> dummy >> m_numModels; _file.ignore(256, '\n');
			m_numModels = count;
			std::cout << "MODELS : " << m_numModels << endl;
			for (int i = 0; i < m_numModels; i++)
			{
				//skip {
				_file.ignore(256, '\n');
				std::cout << "{\n";

				string type;
				_file >> dummy >> type; _file.ignore(256, '\n');
				AIModel* newModel = ModelFactory::makeNewModel(type);
				newModel->Load(_file);

				m_Models.push_back(newModel);

				//skip }
				_file.ignore(256, '\n');
				std::cout << "}\n";
			}
			std::cout << endl << endl;
		}



		//load Textures
		//_file >> dummy >> count; _file.ignore(256, '\n');
		else if (dummy == "TEXTURES")
		{
			m_numTextures = count;

			std::cout << "TEXTURES : " << m_numTextures << endl;
			for (int i = 0; i < m_numTextures; i++)
			{
				//skip {
				_file.ignore(256, '\n');
				std::cout << "{\n";

				m_Textures.push_back(new Texture(_file));

				//skip }
				_file.ignore(256, '\n');
				std::cout << "}\n";
			}
			std::cout << endl << endl;
		}


		//load Shaders
		//_file >> dummy >> count; _file.ignore(256, '\n');

		else if (dummy == "SHADERS")
		{
			//_file >> dummy >> m_numShaders; _file.ignore(256, '\n');
			m_numShaders = count;
			std::cout << "SHADERS : " << m_numShaders << endl;
			for (int i = 0; i < m_numShaders; i++)
			{
				//skip {
				_file.ignore(256, '\n');
				std::cout << "{\n";

				m_Shaders.push_back(new Shader(_file));

				//skip }
				_file.ignore(256, '\n');
				std::cout << "}\n";
			}
			std::cout << endl << endl;
		}


		//_file >> dummy >> count; _file.ignore(256, '\n');

		//load GameObjects
		else if (dummy == "GAMEOBJECTS")
		{
			m_numGameObjects = count;
			//_file >> dummy >> m_numGameObjects; _file.ignore(256, '\n');
			std::cout << "GAMEOBJECTS : " << m_numGameObjects << endl;
			for (int i = 0; i < m_numGameObjects; i++)
			{
				//skip {
				_file.ignore(256, '\n');
				std::cout << "{\n";

				string type;
				_file >> dummy >> type; _file.ignore(256, '\n');
				GameObject* newGO = GameObjectFactory::makeNewGO(type);
				newGO->Load(_file);

				m_GameObjects.push_back(newGO);

				//skip }
				_file.ignore(256, '\n');
				std::cout << "}\n";
			}
			std::cout << endl << endl;
		}

		if (_file.eof())
		{
			std::cout << "<<<<<<<<<<<File Read complete>>>>>>>>>>>>>>\n";
		}
	}
}

void Scene::Init()
{
	//initialise all cameras
	//scene is passed down here to allow for linking of cameras to game objects
	int count = 0;
	for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); ++it)
	{
		(*it)->Init(100, 100, this);// TODO: set correct screen sizes here

		//if a camera is called MAIN
		//this will be the starting camera used
		if ((*it)->GetName() == "MAIN")
		{
			m_useCamera = (*it);
			m_useCameraIndex = count;
		}
		count++;
	}

	//if no MAIN camera just use the first one
	if (!m_useCamera)
	{
		m_useCamera = (*m_Cameras.begin());
		m_useCameraIndex = 0;
	}



	//set up links between everything and GameObjects
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		(*it)->Init(this);
	}

	
}

void Scene::CycleCamera() 
{
	m_useCameraIndex++;
	m_useCameraIndex = m_useCameraIndex % m_numCameras;
	
	auto it = m_Cameras.begin();
	std::advance(it, m_useCameraIndex);

	m_useCamera = *it;

}
void Scene::MouseMoved(float x, float y) {

	if (m_useCamera) 
	{
		ArcballCamera* arcballCam = dynamic_cast<ArcballCamera*>(m_useCamera);
		FPcam* firstPersonCam = dynamic_cast<FPcam*>(m_useCamera);
		OrthoCam* orthoCam = dynamic_cast<OrthoCam*>(m_useCamera);
		if (arcballCam)
		{
			arcballCam->rotateCamera(x, y);
		}
		if (firstPersonCam)
		{
			firstPersonCam->LookAt(x, y);
		}
		if (orthoCam) 
		{
			orthoCam->LookAt(x, y);
		}

	}
}
void Scene::MouseScroll(float s)
{
	if (m_useCamera)
	{
		ArcballCamera* arcballCam = dynamic_cast<ArcballCamera*>(m_useCamera);
		FPcam* firstPersonCam = dynamic_cast<FPcam*>(m_useCamera);
		if (arcballCam)
		{
			arcballCam->incrementRadius(s);
		}
		if (firstPersonCam)
		{
			firstPersonCam->CamZoom(s);
		}
	}

}
void Scene::MoveCam(glm::vec3 direction)
{
	m_useCamera->Move(direction);
	
	FPcam* firstPersonCam = dynamic_cast<FPcam*>(m_useCamera);
	if (firstPersonCam)
	{
		firstPersonCam->moveCam(direction);


	}
}
