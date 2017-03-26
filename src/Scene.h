#ifndef SCENE_H
#define SCENE_H

#include <vector>

class Object;
class Camera;
class Light;

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

#endif SCENE_H