#pragma once

#include "Camera.h"
#include "Light.h"

class Scene
{
public:
	Scene(Object* root, Camera* camera);
	~Scene();

	void addLight(Light* light);

	std::vector<Light*> getLights() { return m_lights; }
	Camera* getCamera() { return m_camera; }
	Object* getRoot() { return m_root; }

private:

	Object* m_root;
	Camera* m_camera;
	std::vector<Light*> m_lights;
};

Scene::Scene(Object* root, Camera* camera) : m_root(root), m_camera(camera)
{

}

Scene::~Scene()
{

}

void Scene::addLight(Light* light) {
	m_lights.push_back(light);
}