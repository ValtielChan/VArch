#include "Object.h"

#include "Component.h"

void Object::update() {

	for (Component* c : m_components)
		c->update(transform.model());

	for (Object* child : m_childs)
		child->update();
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