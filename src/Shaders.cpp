#include "Shaders.h"

Shaders::Shaders() : m_currentShader(nullptr) {

	m_shaders.push_back(new Shader("shaders\\default.vs", "shaders\\default.fs"));
	m_shaders.push_back(new Shader("shaders\\screen.vs", "shaders\\screen.fs"));
	m_shaders.push_back(new Shader("shaders\\phong.vs", "shaders\\phong.fs"));
	m_shaders.push_back(new Shader("shaders\\displacement.vs", "shaders\\displacement.fs"));
	m_shaders.push_back(new Shader("shaders\\selfillumin.vs", "shaders\\selfillumin.fs"));
	m_shaders.push_back(new Shader("shaders\\sample.cs"));
	m_shaders.push_back(new Shader("shaders\\voxelraytracing.cs"));

	useShader(SCREEN);
}

Shaders::~Shaders() {}

void Shaders::useShader(BuiltInShader shader) {

	m_shaders[shader]->use();
	m_currentShader = m_shaders[shader];
}

void Shaders::useShader(unsigned int shader) {

	m_shaders[shader]->use();
	m_currentShader = m_shaders[shader];
}

unsigned int Shaders::addShader(const GLchar* vertexPath, const GLchar* fragmentPath) {
	m_shaders.push_back(new Shader(vertexPath, fragmentPath));
	return m_shaders.size() - 1;
}

unsigned int Shaders::addShader(Shader* shader) {
	m_shaders.push_back(shader);
	return m_shaders.size() - 1;
}

Shader* Shaders::currentShader() { return m_currentShader; }

Shader* Shaders::getShader(BuiltInShader shader) { return m_shaders[shader]; }
Shader* Shaders::getShader(int index) { return m_shaders[index]; }
std::vector<Shader*> Shaders::getShaders() { return m_shaders; }