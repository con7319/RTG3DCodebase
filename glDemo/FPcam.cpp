
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
	UpdateLookAt();
	m_viewMatrix = glm::lookAt(m_pos, m_lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
	m_projectionMatrix = glm::perspective(glm::radians(m_fovY), m_aspect, m_nearPlane, m_farPlane);

}
void FPcam::UpdateLookAt() {
	
	glm::vec3 direction;
	direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	direction.y = sin(glm::radians(m_pitch));
	direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_lookAt = m_pos + glm::normalize(direction);


}

void FPcam::LookAt(float x, float y)
{
	m_yaw += y * m_sensitivity;
	m_pitch -= x * m_sensitivity;

	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;

	UpdateLookAt();

}
void FPcam::CamZoom(float s)
{
	m_fovY += s;

}

void FPcam::moveCam(glm::vec3 direction) 
{

	glm::vec3 forward = glm::normalize(m_lookAt - m_pos);
	forward.y = 0.0f;
	forward = glm::normalize(forward);

	glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

	//glm::vec3 up = glm::normalize(glm::cross(right, forward));

	// Calculate the movement vector based on the camera's orientation
	

	m_pos += forward * direction.z;
	m_pos += right * direction.x;

	m_viewMatrix = glm::lookAt(m_pos, m_lookAt, glm::vec3(0.0f, 1.0f, 0.0f));

	UpdateLookAt(); //Important to update lookat after position change.
}
glm::mat4 FPcam::viewTransform() {

	return m_viewMatrix;
}

// return a const reference the projection transform for the camera
glm::mat4 FPcam::projectionTransform() {

	return m_projectionMatrix;
}