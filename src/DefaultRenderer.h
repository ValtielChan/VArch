#ifndef DEFAULTRENDERER_H
#define DEFAULTRENDERER_H

#include "Renderer.h"

class Scene;

class DefaultRenderer : public Renderer
{
private:
	Scene* m_scene;

public:
	DefaultRenderer(Scene* scene);
	~DefaultRenderer() {}

	void render();
	void lightPass();
	void setWireframe(bool active);
};

#endif //DEFAULTRENDERER_H