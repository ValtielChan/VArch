#ifndef SHADER_H
#define SHADER_H


#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{

public:

	GLuint program;

	Shader(const GLchar *computePath);
	Shader(const GLchar *vertexPath, const GLchar *fragmentPath);
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar *geometryPath);

	void setUniform1i(const char * name, GLint value);
	void setUniform1f(const char * name, float value);
	void setUniform2f(const char* name, glm::vec2 vec);
	void setUniform3f(const char * name, glm::vec3 vec);
	void setUniform4f(const char * name, glm::vec4 vec);
	void setUniformMatrix4fv(const char* name, glm::mat4x4 mat);

	void use();
};

#endif // SHADER_H