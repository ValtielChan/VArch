#pragma once

#include "Renderer.h"
#include "Mesh.h"
#include "Scene.h"

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

DefaultRenderer::DefaultRenderer(Scene* scene) : Renderer(), m_scene(scene) {

}

void DefaultRenderer::render() {

	if (m_scene) {

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Camera* camera = m_scene->getCamera();
		camera->useMVP();

		camera->update();

		lightPass();

		m_scene->getRoot()->update();
	}
}

void DefaultRenderer::lightPass() {

	std::vector<Shader*> shaders = Shaders::getInstance()->getShaders();

	for (Light* light : m_scene->getLights()) {

		for (Shader* shader : shaders) {
			shader->use();
			light->setLightUniform(shader);
		}
	}
}

void DefaultRenderer::setWireframe(bool active)
{
	if (active)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	std::vector<Shader*> shaders = Shaders::getInstance()->getShaders();

	for (Shader* shader : shaders)
		shader->setUniform1i("wireframe", active);
}