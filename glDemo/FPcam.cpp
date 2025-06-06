
#include "FPcam.h"
#include "stringHelp.h"

using namespace std;
using namespace glm;


// initialise camera parameters so it is placed at the origin looking down the -z axis (for a right-handed camera) or +z axis (for a left-handed camera)
FPcam::FPcam() {

	m_type = "FIRST";
	m_pos = this->GetPos();
	m_fovY = 55.0f;
	m_aspect = 1.0f;
	m_nearPlane = 0.1f;
	m_farPlane = 500.0f;

	m_yaw = -90.0f;
	m_pitch = 0.0f;
	m_sensitivity = 0.1f;

}


FPcam::FPcam(glm::vec3 m_pos, glm::vec3 m_lookAt, float _fovY, float _aspect, float _nearPlane, float _farPlane) 
{

	m_type = "FIRST";
	this->m_pos;
	this->m_lookAt;
	this->m_fovY = _fovY;
	this->m_aspect = _aspect;
	this->m_nearPlane = _nearPlane;
	this->m_farPlane = _farPlane;

	this->m_yaw = -90.0f;
	this->m_pitch = 0.0f;
	this->m_sensitivity = 0.1f;




}

void FPcam::Load(ifstream& _file)
{
	StringHelp::String(_file, "NAME", m_name);
	StringHelp::Float3(_file, "POS", m_pos.x, m_pos.y, m_pos.z);
	StringHelp::Float3(_file, "LOOKAT", m_lookAt.x, m_lookAt.y, m_lookAt.z);
	StringHelp::Float(_file, "FOV", m_fov);
	StringHelp::Float(_file, "NEAR", m_near);
	StringHelp::Float(_file, "FAR", m_far);

	
}

void FPcam::Tick(float _dt, float _width, float _height)
{
	m_aspect = _width / _height;
	UpdateForward();
	m_lookAt = m_pos + m_forward;
	m_viewMatrix = glm::lookAt(m_pos, m_lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
	m_projectionMatrix = glm::perspective(glm::radians(m_fovY), m_aspect, m_nearPlane, m_farPlane);

}

void FPcam::UpdateForward()
{

	glm::vec3 newForward;
	newForward.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	newForward.y = sin(glm::radians(m_pitch));
	newForward.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

	// Normalize forward
	m_forward = glm::normalize(newForward);

	// Compute the right vector (perpendicular to forward & world up)
	m_right = glm::normalize(glm::cross(m_forward, glm::vec3(0.0f, 1.0f, 0.0f)));

	// Compute the real up vector (perpendicular to both forward and right)
	m_up = glm::normalize(glm::cross(m_right, m_forward));

}

void FPcam::LookAt(float x, float y)
{
	m_yaw += y * m_sensitivity;
	m_pitch -= x * m_sensitivity;

	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;

	UpdateForward();

	m_viewMatrix = glm::lookAt(m_pos, m_pos + m_forward, m_up);

}
void FPcam::CamZoom(float s)
{
	m_fovY += s;

}

void FPcam::moveCam(glm::vec3 direction)
{
	
	float moveSpeed = 0.01f; // Adjust this value for desired movement speed
	float fixedY = m_pos.y;


	// Interpret input dynamically based on the sign of direction values
	if (direction.z != 0.0f)
		m_pos += m_forward * glm::sign(direction.z) * moveSpeed; // Forward/Backward
	if (direction.x != 0.0f)
		m_pos += m_right * glm::sign(direction.x) * moveSpeed;   // Left/Right
	
	m_pos.y = fixedY;


	// Update the view matrix after movement
	m_viewMatrix = glm::lookAt(m_pos, m_pos + m_forward, m_up);
}

glm::mat4 FPcam::viewTransform() {

	return m_viewMatrix;
}

// return a const reference the projection transform for the camera
glm::mat4 FPcam::projectionTransform() {

	return m_projectionMatrix;
}