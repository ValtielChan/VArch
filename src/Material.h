#pragma once

#include "Shaders.h"

class Material {

protected:

	Shader* m_shader;

public:

	Material(Shader* shader) : m_shader(shader) {}
	~Material() {}

	virtual void setUniform() = 0;

	Shader* getShader() { return m_shader; }
};