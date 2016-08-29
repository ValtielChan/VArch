#include "Camera.h"

Camera::Camera(float screenWidth, float screenHeight, float nearPlane, float farPlane, float zoom)
	: m_screenWidth(screenWidth),
	m_screenHeight(screenHeight),
	m_nearPlane(nearPlane),
	m_farPlane(farPlane),
	m_zoom(zoom),
	m_mouseSensitivity(0.25f),
	m_movementSpeed(6.f),
	m_worldUp(glm::vec3(0, 1.f, 0)),
	m_front(glm::vec3(0, 0, -1.f))
{
	updateProjectionMatrix();
	updateViewMatrix();
	updateVectors();
}

Camera::~Camera()
{
}

void Camera::processMove(CameraMovement direction, float deltaTime)
{
	float velocity = m_movementSpeed * deltaTime;

	if (direction == CameraMovement::FORWARD)
		transform.translate(m_front * velocity);
	if (direction == CameraMovement::BACKWARD)
		transform.translate(-m_front * velocity);
	if (direction == CameraMovement::LEFTWARD)
		transform.translate(-m_right * velocity);
	if (direction == CameraMovement::RIGHTWARD)
		transform.translate(m_right * velocity);

	//std::cout << "Moving" << std::endl;

	updateViewMatrix();
}

void Camera::processLook(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= m_mouseSensitivity;
	yoffset *= m_mouseSensitivity;

	transform.rotate(glm::vec3(0.f, xoffset, 0.f));
	transform.rotate(glm::vec3(yoffset, 0.f, 0.f));

	//std::cout << "x " << transform.rotation().x << " y " << transform.rotation().y << " z " << transform.rotation().z << std::endl;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (transform.rotation().x > 89.0f)
			transform.setRotX(89.0f);
		else if (transform.rotation().x < -89.0f)
			transform.setRotX(-89.0f);
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	updateVectors();
	updateViewMatrix();
}

void Camera::processZoom(float yoffset)
{
	std::cout << "Before Zooming : " << m_zoom << std::endl;

	//if (m_zoom >= 1.f && m_zoom <= 100.f)
		m_zoom -= yoffset;

	/*if (m_zoom <= 1.f)
		m_zoom = 1.f;
	if (m_zoom >= 100.f)
		m_zoom = 100.f;*/

	updateProjectionMatrix();

	std::cout << "Zooming : " << m_zoom << std::endl;
}

void Camera::updateProjectionMatrix()
{
	m_projection = glm::perspective(m_zoom, m_screenWidth / m_screenHeight, m_nearPlane, m_farPlane);
}

void Camera::updateViewMatrix()
{
	m_view = glm::lookAt(transform.position(), transform.position() + m_front, m_up);
}

void Camera::updateVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(transform.rotation().y)) * cos(glm::radians(transform.rotation().x));
	front.y = sin(glm::radians(transform.rotation().x));
	front.z = sin(glm::radians(transform.rotation().y)) * cos(glm::radians(transform.rotation().x));
	m_front = glm::normalize(front);

	// Also re-calculate the Right and Up vector
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::update()
{
	Shader* shader = Shaders::getInstance()->currentShader();

	shader->setUniform3f("viewPos", transform.position());
}

void Camera::useMVP()
{
	MVP* mvp = MVP::getInstance();

	mvp->setView(m_view);
	mvp->setProjection(m_projection);
}