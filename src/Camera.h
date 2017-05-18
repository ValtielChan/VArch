#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Object.h"

class CameraManipulator;

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

	CameraManipulator *m_manipulator;

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
	float sensitivity() { return m_mouseSensitivity; }
	float movementSpeed() { return m_movementSpeed; }
	glm::vec3 front() { return m_front; }
	glm::vec3 right() { return m_right; }
	glm::vec3 up() { return m_up; }
	glm::mat4 view() { return m_view; }
	glm::mat4 projection() { return m_projection; }

	void setManipulator(CameraManipulator* manipulator);
	void setSensitivity(float sensitivity) { m_mouseSensitivity = sensitivity; }
	void setSpeed(float speed) { m_movementSpeed = speed; }

	// Update
	void updateProjectionMatrix();
	void updateViewMatrix();
	void updateVectors(glm::vec3 frontVector);
	void updateVectors();

	void update();
	void updateRayTrace();

	// Use camera view and projection matrix as MVP parameters
	void useMVP();
};

class CameraManipulator
{

public:

	CameraManipulator(Camera* camera) : m_camera(camera) {}
	~CameraManipulator() {}

	virtual void processMove(CameraMovement direction, float deltaTime) = 0;
	virtual void processLook(float xoffset, float yoffset, bool constrainPitch) = 0;

protected:
	Camera* m_camera;
};

#endif