
#include "OrthoCam.h"
#include "stringHelp.h"

using namespace std;
using namespace glm;


// initialise camera parameters so it is placed at the origin looking down the -z axis (for a right-handed camera) or +z axis (for a left-handed camera)
OrthoCam::OrthoCam() {

	m_type = "ORTHO";
	m_pos = glm::vec3(10.0f, 10.0f, 10.0f);
	m_fovY = 55.0f;
	m_aspect = 1.0f;
	m_nearPlane = 0.1f;
	m_farPlane = 500.0f;

	m_yaw = -90.0f;
	m_pitch = 0.0f;
	m_sensitivity = 0.1f;

}


OrthoCam::OrthoCam(glm::vec3 m_pos, glm::vec3 m_lookAt, float _fovY, float _aspect, float _nearPlane, float _farPlane)
{

	m_type = "ORTHO";
	this->m_pos;
	this->m_lookAt;
	this->m_fovY = _fovY;
	this->m_aspect = _aspect;
	this->m_nearPlane = _nearPlane;
	this->m_farPlane = _farPlane;

	this->m_yaw = -90.0f;
	this->m_pitch = 0.0f;
	this->m_sensitivity = 0.5f;




}

void OrthoCam::Load(ifstream& _file)
{
	StringHelp::String(_file, "NAME", m_name);
	StringHelp::Float3(_file, "POS", m_pos.x, m_pos.y, m_pos.z);
	StringHelp::Float3(_file, "LOOKAT", m_lookAt.x, m_lookAt.y, m_lookAt.z);
	StringHelp::Float(_file, "FOV", m_fov);
	StringHelp::Float(_file, "NEAR", m_near);
	StringHelp::Float(_file, "FAR", m_far);


}

void OrthoCam::Tick(float _dt, float _width, float _height) {
	m_width = _width;
	m_height = _height;

	aspect_ratio = _width / _height;

	// Ensure proper camera orbit and position
	glm::vec3 offset = m_pos - m_lookAt; // Calculate offset from the lookAt point
	float radius = glm::length(offset);   // Get distance from the lookAt point

	float orbitAngle = glm::radians(45.0f);  // Convert yaw to radians

	// Update position based on orbit angle, ensuring correct distance (radius)
	m_pos.x = m_lookAt.x + radius * cos(orbitAngle);
	//m_pos.z = m_lookAt.z + radius * sin(orbitAngle);

	// Keep the y-value fixed (or adjust it as needed for scene size)
	//m_pos.y = m_lookAt.y + 10.0f;  // Fixed height above the lookAt point

	// Update the view matrix
	m_viewMatrix = glm::lookAt(m_pos, m_lookAt, glm::vec3(0.0f, 1.0f, 0.0f));

	// Update the projection matrix
	m_projectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 500.0f);

	// Print camera position and lookAt for debugging
	//std::cout << "Camera Position: " << m_pos.x << ", " << m_pos.y << ", " << m_pos.z << std::endl;
	//std::cout << "Camera LookAt: " << m_lookAt.x << ", " << m_lookAt.y << ", " << m_lookAt.z << std::endl;

	// Check if the camera is producing a black screen
	if (m_pos == m_lookAt) {
		std::cout << "Warning: Camera is looking at the same position as the camera position. Adjust the lookAt point." << std::endl;

	}
}

void OrthoCam::UpdateForward()
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

void OrthoCam::LookAt(float x, float y)
{
	// Update yaw for horizontal rotation
	   // Adjust yaw and pitch based on mouse movement
	m_yaw += y * m_sensitivity;
	m_pitch -= x * m_sensitivity;

	// Clamp pitch to prevent flipping
	/*if (m_pitch > 89.0f) m_pitch = 89.0f;
	if (m_pitch < -89.0f) m_pitch = -89.0f;*/

	// Keep yaw within 0 to 360 degrees
	if (m_yaw > 360.0f) m_yaw -= 360.0f;
	if (m_yaw < 0.0f) m_yaw += 360.0f;

	// Calculate new position around `m_lookAt` using spherical coordinates
	float radius = 15.0f; // Fixed orbit radius
	float yawRadians = glm::radians(m_yaw);
	float pitchRadians = glm::radians(m_pitch);

	m_pos.x = m_lookAt.x + radius * cos(pitchRadians) * cos(yawRadians);
	m_pos.y = m_lookAt.y + radius * sin(pitchRadians);
	m_pos.z = m_lookAt.z + radius * cos(pitchRadians) * sin(yawRadians);

	//keep camera within range of world

	if (glm::length(m_pos - m_lookAt) > m_farPlane) {
		m_pos = m_lookAt + glm::normalize(m_pos - m_lookAt) * m_farPlane;
	}

	// Update forward vector
	//UpdateForward();

	// Update view matrix
	m_viewMatrix = glm::lookAt(m_pos, m_lookAt, m_up);

}
void OrthoCam::CamZoom(float s)
{
	m_fovY += s;

}

void OrthoCam::moveCam(glm::vec3 direction)
{

	float moveSpeed = 2.0f; // Adjust this value for desired movement speed
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

glm::mat4 OrthoCam::viewTransform() {

	return m_viewMatrix;
}

// return a const reference the projection transform for the camera
glm::mat4 OrthoCam::projectionTransform() {

	return m_projectionMatrix;
}