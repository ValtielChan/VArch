#include "Object.h"

#include "Component.h"
#include "Skybox.h"

void Object::update(Shader* shader) {

	for (Component* c : m_components)
		c->update(transform.model(), shader);

	for (Object* child : m_childs)
		child->update(shader);
}

void Object::voidUpdate() {

	// Update MVP
	MVP::getInstance()->setModel(transform.model());
	MVP::setUniform();

	for (Component* c : m_components)
		c->update();
}

std::vector<Object*> Object::getChilds() {
	return m_childs;
}

void Object::addChild(Object* child) {
	m_childs.push_back(child);
}

void Object::addComponent(Component* c) {
	m_components.push_back(c);
}

bool Object::isActive() {
	return m_active; // TODO check parents
}

void Object::setActive(bool active) { m_active = active; }