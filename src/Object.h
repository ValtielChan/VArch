#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>

#include "Transform.h"
#include "MVP.h"

class Component;

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
	virtual ~Object();

	void update(Shader* shader = NULL);
	void voidUpdate();
	std::vector<Object*> getChilds();
	void addChild(Object* child);
	void addComponent(Component* c);
	void clearComponents() { m_components.clear(); }
	bool isActive();
	void setActive(bool active);
};

#endif // OBJECT_H
