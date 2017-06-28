#include "Light.h"
#include "Shader.h"
#include "Component.h"

Light::Light() :
	ambient(glm::vec3(0.2f, 0.2f, 0.2f)),
	diffuse(glm::vec3(0.5f, 0.5f, 0.5f)),
	specular(glm::vec3(.5f, .5f, .5f)),
	isDraw(false)
{
}

Light::~Light()
{
}

void Light::update()
{
	if (isDraw) {
		for (Component* c : m_components)
			c->update(transform.model());
	}
}

void Light::voidUpdate()
{
	// Must do nothing
}

void Light::setLightUniform(Shader* shader, int index)
{
	shader->setUniform3f("light.position", transform.position());
	shader->setUniform3f("light.ambient", ambient);
	shader->setUniform3f("light.diffuse", diffuse);
	shader->setUniform3f("light.specular", specular);
}