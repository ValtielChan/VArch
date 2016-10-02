#pragma once

#include <iostream>
#include <vector>

#include "Shader.h"
#include "Singleton.h"

enum BuiltInShader {
	DEFAULT, SCREEN, PHONG, DISPLACEMENT, SELFILLUMIN
};

class Shaders : public Singleton<Shaders>
{
	friend class Singleton<Shaders>;

private:
	Shaders() : m_currentShader(nullptr) {

		m_shaders.push_back(new Shader("shaders\\default.vs", "shaders\\default.fs"));
		m_shaders.push_back(new Shader("shaders\\screen.vs", "shaders\\screen.fs"));
		m_shaders.push_back(new Shader("shaders\\phong.vs", "shaders\\phong.fs"));
		m_shaders.push_back(new Shader("shaders\\displacement.vs", "shaders\\displacement.fs"));
		m_shaders.push_back(new Shader("shaders\\selfillumin.vs", "shaders\\selfillumin.fs"));

		useShader(SCREEN);
	}

	~Shaders() {}

	Shader* m_currentShader;

	// User added shaders
	std::vector<Shader*> m_shaders;

public:

	void useShader(BuiltInShader shader) {

		m_shaders[shader]->use();
		m_currentShader = m_shaders[shader];
	}

	void useShader(unsigned int shader) {

		m_shaders[shader]->use();
		m_currentShader = m_shaders[shader];
	}

	unsigned int addShader(const GLchar* vertexPath, const GLchar* fragmentPath) {
		m_shaders.push_back(new Shader(vertexPath, fragmentPath));
		return m_shaders.size() - 1;
	}

	unsigned int addShader(Shader* shader) {
		m_shaders.push_back(shader);
		return m_shaders.size() - 1;
	}

	Shader* currentShader() { return m_currentShader; }

	Shader* getShader(BuiltInShader shader) { return m_shaders[shader]; }
	Shader* getShader(int index) { return m_shaders[index]; }
	std::vector<Shader*> getShaders() { return m_shaders; }
};