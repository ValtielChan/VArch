#ifndef FREECAM_H
#define FREECAM_H

#include "Camera.h"

class FreeCam : public CameraManipulator
{
public:
	FreeCam(Camera* camera);
	~FreeCam();

	void processMove(CameraMovement direction, float deltaTime);
	void processLook(float xoffset, float yoffset, bool constrainPitch);

private:

};

#endif //FREECAM_H