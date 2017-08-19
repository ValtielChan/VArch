#ifndef DEFAULTRENDERER_H
#define DEFAULTRENDERER_H

#include "Singleton.h"
#include "Renderer.h"
#include "Camera.h"

class Scene;
class Skybox;
class FrameBufferObject;

class DefaultRenderer : public Renderer, public Singleton<DefaultRenderer>
{
	friend class Singleton<DefaultRenderer>;

private:

	DefaultRenderer();
	~DefaultRenderer() {}

	Scene* m_scene;
	Camera* m_camera;
	Skybox* m_skybox;

	//Shadow
	FrameBufferObject* m_shadowMap;

	//Render
	FrameBufferObject* m_fbo;

public:
	

	void render();
	void shadowPass();
	void lightPass();
	void skyboxPass();
	void setWireframe(bool active);

	Camera* getCamera() { return m_camera; }

	void resetViewport() { glViewport(0, 0, m_camera->screenWidth(), m_camera->screenHeight()); }
};

#endif //DEFAULTRENDERER_H