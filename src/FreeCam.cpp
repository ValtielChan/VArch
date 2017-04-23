#include "FreeCam.h"

FreeCam::FreeCam(Camera* camera) : CameraManipulator(camera) 
{
}

FreeCam::~FreeCam()
{
}

void FreeCam::processMove(CameraMovement direction, float deltaTime)
{
	float velocity = m_camera->movementSpeed() * deltaTime;

	if (direction == CameraMovement::FORWARD)
		m_camera->transform.translate(m_camera->front() * velocity);
	if (direction == CameraMovement::BACKWARD)
		m_camera->transform.translate(-m_camera->front() * velocity);
	if (direction == CameraMovement::LEFTWARD)
		m_camera->transform.translate(-m_camera->right() * velocity);
	if (direction == CameraMovement::RIGHTWARD)
		m_camera->transform.translate(m_camera->right() * velocity);

	//std::cout << "Moving" << std::endl;

	m_camera->updateVectors();
	m_camera->updateViewMatrix();
}

void FreeCam::processLook(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= m_camera->sensitivity();
	yoffset *= m_camera->sensitivity();

	m_camera->transform.rotate(glm::vec3(0.f, xoffset, 0.f));
	m_camera->transform.rotate(glm::vec3(yoffset, 0.f, 0.f));

	//std::cout << "x " << transform.rotation().x << " y " << transform.rotation().y << " z " << transform.rotation().z << std::endl;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (m_camera->transform.rotation().x > 89.0f)
			m_camera->transform.setRotX(89.0f);
		else if (m_camera->transform.rotation().x < -89.0f)
			m_camera->transform.setRotX(-89.0f);
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	m_camera->updateVectors();
	m_camera->updateViewMatrix();
}