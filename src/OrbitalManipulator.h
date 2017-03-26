#ifndef ORBITALMANIPULATOR_H
#define ORBITALMANIPULATOR_H

#include "Camera.h"

class OrbitalManipulator : public CameraManipulator
{
public:
	OrbitalManipulator(Camera* camera);
	~OrbitalManipulator();

	void processMove(CameraMovement direction, float deltaTime);
	void processLook(float xoffset, float yoffset, bool constrainPitch);

	void updateCameraPosition();

private:

	float m_phi;
	float m_theta;
	float m_radius;
};

#endif // !ORBITALMANIPULATOR_H