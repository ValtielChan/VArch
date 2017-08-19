#ifndef COMPONENT_H
#define COMPONENT_H

class Shader;

class Component {

public:
	Component() {}
	virtual ~Component() {}

	virtual void update() = 0;
	virtual void update(glm::mat4 model, Shader* shader = NULL) {}
};

#endif //COMPONENT_H