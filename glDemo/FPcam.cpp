
#include "FPcam.h"
#include "stringHelp.h"

using namespace std;
using namespace glm;

//
// Private API
//

// update position, orientation and view matrices when camera rotation and radius is modified
void FPcam::calculateDerivedValues() {

	const float theta_ = glm::radians<float>(m_theta);
	const float phi_ = glm::radians<float>(m_phi);
	
	// calculate position vector
	cameraPos = glm::vec4(sinf(phi_) * cosf(-theta_) * m_radius, sinf(-theta_) * m_radius, cosf(phi_) * cosf(-theta_) * m_radius, 1.0f);

	// calculate orientation basis R
	//R = glm::eulerAngleY(phi_) * glm::eulerAngleX(theta_);

	// calculate view and projection transform matrices
	m_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -m_radius)) * glm::eulerAngleX(-theta_) * glm::eulerAngleY(-phi_);
	m_projectionMatrix = glm::perspective(glm::radians<float>(m_fovY), m_aspect, m_nearPlane, m_farPlane);
}


//
// Public method implementation
//

// ArcballCamera constructors

// initialise camera parameters so it is placed at the origin looking down the -z axis (for a right-handed camera) or +z axis (for a left-handed camera)
FPcam::FPcam() {

	m_type = "FIRST";
	m_theta = 0.0f;
	m_phi = 0.0f;
	m_radius = 0.0f;

	m_fovY = 55.0f;
	m_aspect = 1.0f;
	m_nearPlane = 0.1f;
	m_farPlane = 500.0f;

	//F = ViewFrustum(55.0f, 1.0f, 0.1f, 500.0f);

	// calculate derived values
	calculateDerivedValues();
	//F.calculateWorldCoordPlanes(C, R);
}


// create a camera with orientation <theta, phi> representing Euler angles specified in degrees and Euclidean distance 'init_radius' from the origin.  The frustum / viewplane projection coefficients are defined in init_fovy, specified in degrees spanning the entire vertical field of view angle, init_aspect (w/h ratio), init_nearPlane and init_farPlane.  If init_farPlane = 0.0 (as determined by equalf) then the resulting frustum represents an infinite perspective projection.  This is the default
FPcam::FPcam(float _theta, float _phi, float _radius, float _fovY, float _aspect, float _nearPlane, float _farPlane) {

	m_type = "FIRST";
	this->m_theta = _theta;
	this->m_phi = _phi;
	this->m_radius = std::max<float>(0.0f, _radius);

	this->m_fovY = _fovY;
	this->m_aspect = _aspect;
	this->m_nearPlane = _nearPlane;
	this->m_farPlane = _farPlane;

	//F = ViewFrustum(init_fovy, init_aspect, init_nearPlane, init_farPlane);

	// calculate derived values
	calculateDerivedValues();
	//F.calculateWorldCoordPlanes(C, R);
}

void FPcam::Load(ifstream& _file)
{
	StringHelp::String(_file, "NAME", m_name);
	StringHelp::Float3(_file, "POS", m_pos.x, m_pos.y, m_pos.z);
	StringHelp::Float3(_file, "LOOKAT", m_lookAt.x, m_lookAt.y, m_lookAt.z);
	StringHelp::Float(_file, "THETA", m_theta);
	StringHelp::Float(_file, "PHI", m_phi);
	StringHelp::Float(_file, "RADIUS", m_radius);
	StringHelp::Float(_file, "FOV", m_fov);
	StringHelp::Float(_file, "NEAR", m_near);
	StringHelp::Float(_file, "FAR", m_far);
}
#pragma region Accessor methods for stored values

// return the pivot rotation around the x axis (theta) in degrees
float FPcam::getTheta() {

	return m_theta;
}

// return the pivot rotation around the y axis (phi) in degrees
float FPcam::getPhi() {

	return m_phi;
}

void FPcam::rotateCamera(float _dTheta, float _dPhi) {

	m_theta += _dTheta;
	m_phi += _dPhi;

	calculateDerivedValues();
}

float FPcam::getRadius() {

	return m_radius;
}

void FPcam::scaleRadius(float _s) {

	m_radius *= _s;
	calculateDerivedValues();
}

void FPcam::incrementRadius(float _i) {

	m_radius = std::max<float>(m_radius + _i, 0.0f);
	calculateDerivedValues();
}

float FPcam::getFovY() {

	return m_fovY;
}

void FPcam::setFovY(float _fovY) {

	this->m_fovY = _fovY;
	calculateDerivedValues();
}

float FPcam::getAspect() {

	return m_aspect;
}

void FPcam::setAspect(float _aspect) {

	this->m_aspect = _aspect;
	calculateDerivedValues();
}

float FPcam::getNearPlaneDistance() {

	return m_nearPlane;
}

void FPcam::setNearPlaneDistance(float _nearPlaneDistance) {

	this->m_nearPlane = _nearPlaneDistance;
	calculateDerivedValues();
}

float FPcam::getFarPlaneDistance() {

	return m_farPlane;
}

void FPcam::setFarPlaneDistance(float _farPlaneDistance) {

	this->m_farPlane = _farPlaneDistance;
	calculateDerivedValues();
}

#pragma endregion


#pragma region Accessor methods for derived values

//return the camera location in world coordinate space
glm::vec4 FPcam::getPosition() {

	return cameraPos;
}

// return a const reference to the camera's orientation matrix in world coordinate space
//glm::mat4 ArcballCamera::getOrientationBasis() {
//
//	return R;
//}

// return a const reference to the view transform matrix for the camera
glm::mat4 FPcam::viewTransform() {

	return m_viewMatrix;
}

// return a const reference the projection transform for the camera
glm::mat4 FPcam::projectionTransform() {

	return m_projectionMatrix;
}

#pragma endregion