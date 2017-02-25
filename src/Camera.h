#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

#include "Object.h"
#include "MVP.h"

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFTWARD,
	RIGHTWARD
};

class Camera : public Object
{
private:

	glm::mat4 m_view;
	glm::mat4 m_projection;
	glm::mat4 m_permissiveProjection;

	glm::vec3 m_front; 
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;

	float m_screenWidth, m_screenHeight;
	float m_nearPlane, m_farPlane;
	float m_zoom;

	float m_mouseSensitivity;
	float m_movementSpeed;

public:

	Camera(float screenWidth = 16.f, float screenHeight = 9.f, float nearPlane = 0.1f, float farPlane = 1000.f, float zoom = 60.f);
	~Camera();

	bool isInFrustum(const glm::vec3 & position, bool permissive = false);

	// Movements
	void processMove(CameraMovement direction, float deltaTime);
	void processLook(float xoffset, float yoffset, bool constrainPitch = true);
	void processZoom(float yoffset);

	// Getter/Setter
	float nearPlane() { return m_nearPlane; }
	float farPlane() { return m_farPlane; }
	float zoom() { return m_zoom; }
	glm::vec3 front() { return m_front; }
	glm::mat4 view() { return m_view; }
	glm::mat4 projection() { return m_projection; }

	// Update
	void updateProjectionMatrix();
	void updateViewMatrix();
	void updateVectors();

	void update();
	void updateRayTrace();

	// Use camera view and projection matrix as MVP parameters
	void useMVP();
};

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

bool Camera::isInFrustum(const glm::vec3 & position, bool permissive)
{
	glm::vec4 pos(position, 1);
	glm::vec4 tranPos;

	glm::vec4 camSpace = m_view * pos;

	glm::mat4x4 &proj = permissive ? m_permissiveProjection : m_projection;

	glm::vec4 row0 = glm::row(proj, 0);
	glm::vec4 row1 = glm::row(proj, 1);
	glm::vec4 row2 = glm::row(proj, 2);
	glm::vec4 row3 = glm::row(proj, 3);

	tranPos.x = glm::dot(row0, camSpace);
	tranPos.y = glm::dot(row1, camSpace);
	tranPos.z = glm::dot(row2, camSpace);
	tranPos.w = glm::dot(row3, camSpace);

	tranPos = glm::normalize(tranPos);

	if (tranPos.x >= -tranPos.w && tranPos.x <= tranPos.w
		&& tranPos.y >= -tranPos.w && tranPos.y <= tranPos.w
		/*&& tranPos.z >= -tranPos.w && tranPos.z <= tranPos.w*/) {
		return true;
	}
	else {
		return false;
	}
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
	m_permissiveProjection = glm::perspective(m_zoom, (m_screenWidth / m_screenHeight) * 2.f, m_nearPlane, m_farPlane);
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
	updateVectors();
	updateViewMatrix();

	std::vector<Shader*> shaders = Shaders::getInstance()->getShaders();
	
	// Regular Pipeline
	for (Shader* shader : shaders) {
		shader->use();
		shader->setUniform3f("viewPos", transform.position());
	}
}

void Camera::updateRayTrace() {
	
	// RayTracing ComputeShader
	Shader* shader = Shaders::getInstance()->getShader(VOXEL_RAYTRACING);
	shader->use();

	float focalLength = 1.0f / std::tan(m_zoom / 2);

	glm::mat4 matrix = glm::transpose(glm::inverse(m_view));

	glm::vec3 ray00;
	ray00 = glm::vec3(-1, -1, -focalLength);
	ray00 = glm::vec3(glm::vec4(ray00, 0) * matrix);

	glm::vec3 ray10;
	ray10 = glm::vec3(1, -1, -focalLength);
	ray10 = glm::vec3(glm::vec4(ray10, 0) * matrix);

	glm::vec3 ray01;
	ray01 = glm::vec3(-1, 1, -focalLength);
	ray01 = glm::vec3(glm::vec4(ray01, 0) * matrix);

	glm::vec3 ray11;
	ray11 = glm::vec3(1, 1, -focalLength);
	ray11 = glm::vec3(glm::vec4(ray11, 0) * matrix);

	//glm::mat4 invViewProjMat = glm::inverse(m_view * m_projection);

	/*glm::vec4 ray00 = glm::vec4(0, -1, -1, 1) * invViewProjMat;
	ray00 /= ray00.w;
	ray00 -= glm::vec4(transform.position(), 1);

	glm::vec4 ray10 = glm::vec4(0, -1, 1, 1) * invViewProjMat;
	ray10 /= ray10.w;
	ray10 -= glm::vec4(transform.position(), 1);

	glm::vec4 ray01 = glm::vec4(0, 1, -1, 1) * invViewProjMat;
	ray01 /= ray01.w;
	ray01 -= glm::vec4(transform.position(), 1);

	glm::vec4 ray11 = glm::vec4(0, 1, 1, 1) * invViewProjMat;
	ray11 /= ray11.w;
	ray11 -= glm::vec4(transform.position(), 1);*/

	glm::vec4 rayOrigin = glm::vec4(transform.position(), 0) * glm::transpose(m_view * m_projection);
	shader->setUniform3f("viewPos", transform.position());

	shader->setUniform3f("ray00", glm::normalize(glm::vec3(ray00)));
	shader->setUniform3f("ray10", glm::normalize(glm::vec3(ray10)));
	shader->setUniform3f("ray01", glm::normalize(glm::vec3(ray01)));
	shader->setUniform3f("ray11", glm::normalize(glm::vec3(ray11)));
}

void Camera::useMVP()
{
	MVP* mvp = MVP::getInstance();

	mvp->setView(m_view);
	mvp->setProjection(m_projection);
}
