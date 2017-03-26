#include "OrbitalManipulator.h"



OrbitalManipulator::OrbitalManipulator(Camera* camera) 
	: CameraManipulator(camera),
	m_phi(0.f),
	m_theta(0.f),
	m_radius(2.f)
{
}


OrbitalManipulator::~OrbitalManipulator()
{
}

void OrbitalManipulator::processMove(CameraMovement direction, float deltaTime)
{
	float velocity = m_camera->movementSpeed() * deltaTime;

	if (direction == CameraMovement::FORWARD)
		m_radius -= velocity;
		//m_camera->transform.translate(m_camera->front() * velocity);
	if (direction == CameraMovement::BACKWARD)
		m_radius += velocity;
		//m_camera->transform.translate(-m_camera->front() * velocity);

	updateCameraPosition();

	m_camera->updateVectors();
	m_camera->updateViewMatrix();
}

void OrbitalManipulator::processLook(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= m_camera->sensitivity();
	yoffset *= m_camera->sensitivity();

	m_phi += xoffset;
	//m_theta += yoffset;

	updateCameraPosition();

	m_camera->updateVectors();
	m_camera->updateViewMatrix();
}

void OrbitalManipulator::updateCameraPosition()
{
	m_camera->transform.setPosition(glm::vec3(
		m_radius * sin(m_phi), //* sin(m_theta),
		0, //m_radius * cos(m_theta),
		m_radius * cos(m_phi) //* sin(m_theta)
	));
}
