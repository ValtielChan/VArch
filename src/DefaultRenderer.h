#ifndef DEFAULTRENDERER_H
#define DEFAULTRENDERER_H

#include "Renderer.h"

class Scene;
class Skybox;
class Camera;

class DefaultRenderer : public Renderer
{
private:
	Scene* m_scene;
	Camera* m_camera;
	Skybox* m_skybox;

public:
	DefaultRenderer(Scene* scene);
	~DefaultRenderer() {}

	void render();
	void lightPass();
	void skyboxPass();
	void setWireframe(bool active);
};

#endif //DEFAULTRENDERER_H