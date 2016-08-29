#pragma once

#include <iostream>
#include <vector>

#include "Transform.h"
#include "Component.h"
#include "MVP.h"

class Object
{
protected:

	Object* m_parent;
	std::vector<Object*> m_childs;
	std::vector <Component*> m_components;

	bool m_active;

public:

	Transform transform;

	Object() {}
	~Object() {}

	void update() {

		for (Component* c : m_components)
			c->update(transform.model());
	}

	void voidUpdate() {

		// Update MVP
		MVP::getInstance()->setModel(transform.model());
		MVP::setUniform();

		for (Component* c : m_components)
			c->update();
	}

	std::vector<Object*> getChilds() {
		return m_childs;
	}

	void addChild(Object* child) {
		m_childs.push_back(child);
	}

	void addComponent(Component* c) {
		m_components.push_back(c);
	}

	bool isActive() {
		return m_active; // TODO check parents
	}

	void setActive(bool active) { m_active = active; }
};

