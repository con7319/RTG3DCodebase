#pragma once

#include "core.h"
#include "Camera.h"




class OrthoCam : public Camera {

private:


	// Projection / frustum values
	float				m_fovY, m_aspect, m_nearPlane, m_farPlane;

	float m_yaw;
	float m_pitch;
	float m_sensitivity;
	float m_width;
	float m_height;

	glm::vec3 m_forward;
	glm::vec3 m_right;
	glm::vec3 m_up;



	// view transform matrix for camera's current position and orientation - maps from world to eye coordinate space
	glm::mat4			m_viewMatrix;

	// projection transform matrix
	glm::mat4			m_projectionMatrix;




public:

	// Constructors
	OrthoCam(); // initialise camera parameters so it is placed at the origin looking down the -z axis (for a right-handed camera) or +z axis (for a left-handed camera)

	OrthoCam(glm::vec3 m_pos, glm::vec3 m_lookAt, float _fovY, float _aspect, float _nearPlane, float _farPlane);
	// create a camera with orientation <theta, phi> representing Euler angles specified in degrees and Euclidean distance 'init_radius' from the origin.  The frustum / viewplane projection coefficients are defined in init_fovy, specified in degrees spanning the entire vertical field of view angle, init_aspect (w/h ratio), init_nearPlane and init_farPlane.  If init_farPlane = 0.0 (as determined by equalf) then the resulting frustum represents an infinite perspective projection.  This is the default

	void Load(ifstream& _file);

	void Tick(float _dt, float _width, float _height);


	void UpdateForward();
	void LookAt(float x, float y);

	void CamZoom(float s);

	void moveCam(glm::vec3 direction);

	glm::mat4 viewTransform();

	glm::mat4 projectionTransform();

};
