#include "Scene.h"

#include "Camera.h"
#include "Light.h"

Scene::Scene(Object* root, Camera* camera) : m_root(root), m_camera(camera)
{}

Scene::~Scene()
{}

void Scene::addLight(Light* light) {
	m_lights.push_back(light);
}