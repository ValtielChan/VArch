#ifndef LIGHT_H
#define LIGHT_H

#include "Object.h"

class Light : public Object
{
public:

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	bool isDraw;

	Light();
	~Light();

	void update();
	void voidUpdate();
	virtual void setLightUniform(Shader* shader);

private:

};

#endif // !LIGHT_H