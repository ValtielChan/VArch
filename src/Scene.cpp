#include "Scene.h"

#include "Camera.h"
#include "Light.h"

Scene::Scene()
{
	m_root = new Object;
	m_camera = new Camera((float)1920, (float)1080, 0.01f, 100000.f, 0.90f);
}

Scene::~Scene()
{}

void Scene::addLight(Light* light) {
	m_lights.push_back(light);
}

void Scene::addObject(Object* object)
{
	m_root->addChild(object);
}

void Scene::clearScene()
{
	delete(m_root);
	m_root = new Object;

	m_lights.clear();
}
