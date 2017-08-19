#include "DefaultRenderer.h"

#include "Scene.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "Shaders.h"
#include "Skybox.h"
#include "FrameBufferObject.h"

DefaultRenderer::DefaultRenderer() : Renderer() {

	m_scene = Scene::getInstance();
	m_skybox = new Skybox();
	m_camera = m_scene->getCamera();
}

void DefaultRenderer::render() {

	if (m_scene) {

		/*glCullFace(GL_FRONT);
		shadowPass();
		glCullFace(GL_BACK);*/

		glClearColor(0.1f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDepthMask(GL_FALSE);
		skyboxPass();
		glDepthMask(GL_TRUE);

		m_camera->useMVP();
		m_camera->update();

		lightPass();

		m_scene->getRoot()->update();
	}
}

void DefaultRenderer::shadowPass()
{
	// Light space matrix
	float near_plane = 1.f, far_plane = 50.f;
	float frustum = 10.f;
	glm::mat4 lightProjection = glm::ortho(-frustum, frustum, -frustum, frustum, near_plane, far_plane);

	// Get the only dir light
	glm::vec3 lightDir;

	for (Light* light : m_scene->getLights()) {

		DirectionalLight* dl = dynamic_cast<DirectionalLight*>(light);

		if (dl != NULL) {
			lightDir = (dl->direction * -1.0f) * 10.0f;
		}
	}

	glm::mat4 lightView = glm::lookAt(lightDir,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	Shader *depthShader = Shaders::getInstance()->getShader(BuiltInShader::DEPTH);
	depthShader->use();

	MVP::getInstance()->setView(lightView);
	MVP::getInstance()->setProjection(lightProjection);

	// Render to FBO
	if (!m_shadowMap)
		m_shadowMap = new FrameBufferObject(10240, 10240, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_ATTACHMENT);

	m_shadowMap->bind();
	glClear(GL_DEPTH_BUFFER_BIT);

	// Render
	m_scene->getRoot()->update(depthShader);

	m_shadowMap->unbind();
	resetViewport();

	// Set unforms for the real render
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_shadowMap->texture());

	std::vector<Shader*> shaders = Shaders::getInstance()->getShaders();
	for (Shader* shader : shaders) {

		shader->use();

		shader->setUniform1i("shadowMap", 0);
		shader->setUniformMatrix4fv("lightSpaceMatrix", lightSpaceMatrix);
	}

	// Debug purpose
	renderToQuad(m_shadowMap->texture());
}

void DefaultRenderer::lightPass() {

	std::vector<Shader*> shaders = Shaders::getInstance()->getShaders();

	int pointLightIndex = 0;

	// Set all light uniforms for each shader
	for (Shader* shader : shaders) {

		for (Light* light : m_scene->getLights()) {

			PointLight* pl = dynamic_cast<PointLight*>(light);

			shader->use();

			light->setLightUniform(shader, pointLightIndex);

			// If pointLight type, increment index
			if (pl != NULL) 
				pointLightIndex++;
		}

		shader->setUniform1i("nbPointLights", pointLightIndex);
		pointLightIndex = 0;
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
