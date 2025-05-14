
#include "core.h"
#include "TextureLoader.h"
#include "ArcballCamera.h"
#include "GUClock.h"
#include "PrincipleAxes.h"
#include "shader_setup.h"
#include "helper.h"
#include "AIMesh.h"
#include "Cube.h"
#include "Scene.h"


using namespace std;
using namespace glm;


#pragma region Global variables

GUClock* g_gameClock = nullptr;

// Mouse tracking
bool				g_mouseDown = false;
double				g_prevMouseX, g_prevMouseY;


//Global Game Object
Scene* g_Scene = nullptr;

// Window size
const unsigned int g_initWidth = 512;
const unsigned int g_initHeight = 512;

float _initWidth = g_initWidth;
float _initHeight = g_initHeight;


//booleans for key input
bool wDown = false;
bool aDown = false;
bool sDown = false;
bool dDown = false;

//struct for keystate and direction of movement
struct KeyState {
	bool forward;
	bool backward;
	bool left;
	bool right;
	bool forLeft;
	bool forRight;
	bool backLeft;
	bool backRight;


	KeyState() = default;
};

KeyState m_keyState;

#pragma endregion


// Function prototypes
void renderScene();
void updateScene();
void resizeWindow(GLFWwindow* _window, int _width, int _height);
void keyboardHandler(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);
void mouseMoveHandler(GLFWwindow* _window, double _xpos, double _ypos);
void mouseButtonHandler(GLFWwindow* _window, int _button, int _action, int _mods);
void mouseScrollHandler(GLFWwindow* _window, double _xoffset, double _yoffset);
void mouseEnterHandler(GLFWwindow* _window, int _entered);

KeyState updateKeyState(KeyState& keyState);
void GetDir(KeyState Key);

int main()
{
	//
	// 1. Initialisation
	//

	g_gameClock = new GUClock();

#pragma region OpenGL and window setup

	// Initialise glfw and setup window
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_TRUE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);

	GLFWwindow* window = glfwCreateWindow(g_initWidth, g_initHeight, "GDV5001", NULL, NULL);

	// Check window was created successfully
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window!\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	// Set callback functions to handle different events
	glfwSetFramebufferSizeCallback(window, resizeWindow); // resize window callback
	glfwSetKeyCallback(window, keyboardHandler); // Keyboard input callback
	glfwSetCursorPosCallback(window, mouseMoveHandler);
	glfwSetMouseButtonCallback(window, mouseButtonHandler);
	glfwSetScrollCallback(window, mouseScrollHandler);
	glfwSetCursorEnterCallback(window, mouseEnterHandler);

	// Initialise glew
	glewInit();


	// Setup window's initial size
	resizeWindow(window, g_initWidth, g_initHeight);

#pragma endregion

	// Initialise scene - geometry and shaders etc
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // setup background colour to be black
	glClearDepth(1.0f);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	
	//
	//Set up Scene class
	//

	g_Scene = new Scene();

	ifstream manifest;
	manifest.open("manifest.txt");

	g_Scene->Load(manifest);
	g_Scene->Init();

	manifest.close();


	//
	// Main loop
	// 

	while (!glfwWindowShouldClose(window))
	{
		updateScene();
		renderScene();						// Render into the current buffer
		glfwSwapBuffers(window);			// Displays what was just rendered (using double buffering).

		glfwPollEvents();					// Use this version when animating as fast as possible

		// update window title
		char timingString[256];
		sprintf_s(timingString, 256, "CIS5013: Average fps: %.0f; Average spf: %f", g_gameClock->averageFPS(), g_gameClock->averageSPF() / 1000.0f);
		glfwSetWindowTitle(window, timingString);
	}

	glfwTerminate();

	if (g_gameClock)
	{
		g_gameClock->stop();
		g_gameClock->reportTimingData();
	}

	return 0;
}


// renderScene - function to render the current scene
void renderScene()
{
	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	g_Scene->Render();
}

// Function called to animate elements in the scene
void updateScene() 
{
	float tDelta = 0.0f;

	if (g_gameClock) {

		g_gameClock->tick();
		tDelta = (float)g_gameClock->gameTimeDelta();
	}
	
	GetDir(m_keyState);
	g_Scene->Update(tDelta, _initWidth, _initHeight);
	

}


#pragma region Event handler functions
//none of this is currently passed to the Game object
//probably a good idea to do that

// Function to call when window resized
void resizeWindow(GLFWwindow* _window, int _width, int _height)
{
	
	
	_initWidth = _width;
	_initHeight = _height;

	glViewport(0, 0, _width, _height);		// Draw into entire window
}


// Function to call to handle keyboard input
void keyboardHandler(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
{
	if (_action == GLFW_PRESS) {

		// check which key was pressed...
		switch (_key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(_window, true);
			break;

		case GLFW_KEY_SPACE:
			
			break;
		case GLFW_KEY_Q:
			g_Scene->CycleCamera();
			break;
		case GLFW_KEY_W:
			wDown = true;
			updateKeyState(m_keyState);
			
			break;
		case GLFW_KEY_A:
			aDown = true;
			updateKeyState(m_keyState);
			
			break;
		case GLFW_KEY_S:
			sDown = true;
			updateKeyState(m_keyState);
			
			break;
		case GLFW_KEY_D:
			dDown = true;
			updateKeyState(m_keyState);
			
			break;
		case GLFW_KEY_F:
			g_Scene->SpotLightsLookAt();
			break;
		default:
		{
		}
		}
	}
	else if (_action == GLFW_RELEASE) 
	{
		// handle key release events
		switch (_key)
		{
		case GLFW_KEY_W:
			wDown = false;
			updateKeyState(m_keyState);
			break;
		case GLFW_KEY_A:
			aDown = false;
			updateKeyState(m_keyState);
			break;
		case GLFW_KEY_S:
			sDown = false;
			updateKeyState(m_keyState);
			break;
		case GLFW_KEY_D:
			dDown = false;
			updateKeyState(m_keyState);
			break;
		default:
			{
			}
		}
	}
}
KeyState updateKeyState(KeyState& keyState)
{
	keyState.forward = wDown;
	keyState.left = aDown;
	keyState.backward = sDown;
	keyState.right = dDown;

	// Diagonal keys:
	keyState.forLeft = keyState.forward && keyState.left;
	keyState.forRight = keyState.forward && keyState.right;
	keyState.backLeft = keyState.backward && keyState.left;
	keyState.backRight = keyState.backward && keyState.right;

	return keyState;
}
void GetDir(KeyState Key)
{
	//translate keystate into a vector relating to movement

	//updateKeyState(Key);
	if (Key.forward)
	{
		glm::vec3 forces(0.0f, 0.0f, 1.0);
		g_Scene->MoveCam(forces * 0.0005f);
	}
	if (Key.backward)
	{
		glm::vec3 forces(0.0f, 0.0f, -1.0);
		g_Scene->MoveCam(forces * 0.0005f);
	}
	if (Key.left)
	{
		glm::vec3 forces(-1.0, 0.0f, 0.0f);
		g_Scene->MoveCam(forces * 0.0005f);
	}
	if (Key.right)
	{
		glm::vec3 forces(1.0, 0.0f, 0.0f);
		g_Scene->MoveCam(forces * 0.0005f);
	}
	if (Key.backLeft)
	{
		glm::vec3 forces(-1.0 / 2, 0.0f, -1.0 / 2);
		g_Scene->MoveCam(forces * 0.0005f);
	}
	if (Key.backRight)
	{
		glm::vec3 forces(1.0 / 2, 0.0f, -1.0 / 2);
		g_Scene->MoveCam(forces * 0.0005f);
	}
	if (Key.forLeft)
	{
		glm::vec3 forces(-1.0 / 2, 0.0f, 1.0 / 2);
		g_Scene->MoveCam(forces * 0.0005f);
	}
	if (Key.forRight)
	{
		glm::vec3 forces(1.0 / 2, 0.0f, 1.0 / 2);
		g_Scene->MoveCam(forces * 0.0005f);
	}
}
void mouseMoveHandler(GLFWwindow* _window, double _xpos, double _ypos) 
{
	if (g_mouseDown) {

		//float tDelta = gameClock->gameTimeDelta();

		float dx = float(_xpos - g_prevMouseX);// *360.0f * tDelta;
		float dy = float(_ypos - g_prevMouseY);// *360.0f * tDelta;

		
		g_Scene->MouseMoved(-dy, -dx);

		g_prevMouseX = _xpos;
		g_prevMouseY = _ypos;
	}
}

void mouseButtonHandler(GLFWwindow* _window, int _button, int _action, int _mods) 
{
	if (_button == GLFW_MOUSE_BUTTON_LEFT) 
	{
		if (_action == GLFW_PRESS) 
		{
			g_mouseDown = true;
			glfwGetCursorPos(_window, &g_prevMouseX, &g_prevMouseY);
		}
		else if (_action == GLFW_RELEASE) 
		{
			g_mouseDown = false;
		}
	}
}

void mouseScrollHandler(GLFWwindow* _window, double _xoffset, double _yoffset) {


	if (g_Scene)
	{
		if (_yoffset < 0.0)
			g_Scene->MouseScroll(1.0f);
		else if (_yoffset > 0.0)
			g_Scene->MouseScroll(-1.0f);
	}
}


void mouseEnterHandler(GLFWwindow* _window, int _entered) 
{
}

void SpotLookat()
{
	g_Scene->SpotLightsLookAt();

}
#pragma endregion