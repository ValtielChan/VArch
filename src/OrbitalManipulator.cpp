#include "OrbitalManipulator.h"



OrbitalManipulator::OrbitalManipulator(Camera* camera) 
	: CameraManipulator(camera),
	m_phi(0.f),
	m_theta(0.f),
	m_radius(4.f)
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
		
	if (direction == CameraMovement::BACKWARD)
		m_radius += velocity;



	processLook(0, 0, false);
}

void OrbitalManipulator::processLook(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= m_camera->sensitivity();
	yoffset *= m_camera->sensitivity();

	m_camera->transform.translate(-m_camera->up() * yoffset);
	m_camera->transform.translate(-m_camera->right() * xoffset);

	glm::vec3 front = glm::normalize(glm::vec3(0) - m_camera->transform.position());
	m_camera->transform.setPosition(-front * m_radius);

	m_camera->updateVectors(front);
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
