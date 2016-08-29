#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

	Camera(float screenWidth = 16.f, float screenHeight = 9.f, float nearPlane = 0.1f, float farPlane = 1000.f, float zoom = 46.f);
	~Camera();

	// Movements
	void processMove(CameraMovement direction, float deltaTime);
	void processLook(float xoffset, float yoffset, bool constrainPitch = true);
	void processZoom(float yoffset);

	// Getter/Setter
	float nearPlane() { return m_nearPlane; }
	float farPlane() { return m_farPlane; }
	float zoom() { return m_zoom; }
	glm::mat4 view() { return m_view; }
	glm::mat4 projection() { return m_projection; }

	// Update
	void updateProjectionMatrix();
	void updateViewMatrix();
	void updateVectors();

	void update();

	// Use camera view and projection matrix as MVP parameters
	void useMVP();
};