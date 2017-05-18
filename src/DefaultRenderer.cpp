#include "DefaultRenderer.h"

#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Shaders.h"
#include "Skybox.h"

DefaultRenderer::DefaultRenderer(Scene* scene) : Renderer(), m_scene(scene) {

	m_skybox = new Skybox();
}

void DefaultRenderer::render() {

	if (m_scene) {

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_camera = m_scene->getCamera();

		glDepthMask(GL_FALSE);
		skyboxPass();
		glDepthMask(GL_TRUE);

		m_camera->useMVP();
		m_camera->update();

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

void DefaultRenderer::skyboxPass()
{
	MVP* mvp = MVP::getInstance();

	mvp->setView(glm::mat4(glm::mat3(m_camera->view())));
	mvp->setProjection(m_camera->projection());

	m_skybox->update();
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