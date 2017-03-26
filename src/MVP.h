#ifndef MVP_H
#define MVP_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Singleton.h"

class Shader;

class MVP : public Singleton<MVP> {

	friend class Singleton<MVP>;

private:

	MVP() {}
	~MVP() {}

	glm::mat4 m_model;
	glm::mat4 m_view;
	glm::mat4 m_projection;

public:

	glm::mat4 model() { return m_model; }
	glm::mat4 view() { return m_view; }
	glm::mat4 projection() { return m_projection; }

	void setModel(glm::mat4 model) { m_model = model; }
	void setView(glm::mat4 view) { m_view = view; }
	void setProjection(glm::mat4 projection) { m_projection = projection; }

	void setMVP(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
	static void setUniform();
	static void setUniform(Shader* shader);
};

#endif // !MVP_H