#ifndef SHADERS_H
#define SHADERS_H


#include <iostream>
#include <vector>

#include "Singleton.h"
#include "Shader.h"

enum BuiltInShader {
	DEFAULT, SCREEN, PHONG, DISPLACEMENT, SELFILLUMIN, COMPUTE_SAMPLE, VOXEL_RAYTRACING, SKYBOX, DEPTH
};

class Shaders : public Singleton<Shaders>
{
	friend class Singleton<Shaders>;

private:
	Shaders();
	~Shaders();

	Shader* m_currentShader;

	// User added shaders
	std::vector<Shader*> m_shaders;

public:

	void useShader(BuiltInShader shader);

	void useShader(unsigned int shader);

	unsigned int addShader(const GLchar* vertexPath, const GLchar* fragmentPath);

	unsigned int addShader(Shader* shader);

	Shader* currentShader();

	Shader* getShader(BuiltInShader shader);
	Shader* getShader(int index);
	std::vector<Shader*> getShaders();
};

#endif // !SHADERS_H
