#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "Singleton.h"

class Object;
class Camera;
class Light;

class Scene : public Singleton<Scene>
{

	friend class Singleton<Scene>;

public:
	
	void addLight(Light* light);
	std::vector<Light*> getLights() { return m_lights; }
	Camera* getCamera() { return m_camera; }
	void setCamera(Camera* camera) { m_camera = camera; }
	Object* getRoot() { return m_root; }
	void addObject(Object* object);

	void clearScene();

private:

	Scene();
	~Scene();

	Object* m_root;
	Camera* m_camera;
	std::vector<Light*> m_lights;
};

#endif SCENE_H