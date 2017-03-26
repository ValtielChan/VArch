#pragma once

// Std. Includes
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shaders.h"
#include "Camera.h"
#include "MVP.h"
#include "DirectionalLight.h"
#include "Mesh.h"
#include "PhongMaterial.h"
#include "DisplacementPhongMaterial.h"
#include "QTNode.h"
#include "DefaultRenderer.h"
#include "ComputeShaderTexture.h"
#include "VoxelOctree.h"
#include "Texture1D_3F.h"
#include "Texture1D_4I.h"
#include "Noise.h"
#include "Scene.h"
#include "ColorTable.h"
#include "HeightMap.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Properties
GLuint screenWidth = 1280, screenHeight = 720;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();


Camera* camera = new Camera((float)screenWidth, (float)screenHeight, 0.1f, 500.f, 0.45f);
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// The MAIN function, from here we start our application and run our Game loop
int main()
{
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "VArch", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Octree test
	TextureRGB* colorMap;
	NormalMap* normalMap;
	ColorTable colorTable = ColorTable::Nature(128);

	NoiseProperties np = NoiseProperties(1, 1, 2);
	HeightMap* heightMap = new HeightMap(150, 150);
	heightMap->generateSimplex(&np);
	heightMap->transformInterval(-1.f, 1.f);

	colorMap = heightMap->generateColorMap(colorTable);
	normalMap = heightMap->generateNormalMap();

	VoxelOctree* octree = new VoxelOctree(glm::vec3(0, 0, 0));
	octree->buildTerrain(heightMap, colorMap, normalMap);
	octree->rootUpdateNeighbors();

	std::vector<Cell*> GLSLArray;
	octree->GLSLArray(GLSLArray);

	camera->transform.setPosition(glm::vec3(0, 0, 0));

	ComputeShaderTexture texture(screenWidth, screenHeight);
	GLuint tex_output = texture.genGLTexture(0);

	Shader* compute = Shaders::getInstance()->getShader(VOXEL_RAYTRACING);

	Renderer renderer;

	/// Set textures and compute shader uniforms
	Texture1D_3F minTexture(GLSLArray.size());
	Texture1D_3F maxTexture(GLSLArray.size());
	Texture1D_3F colorTexture(GLSLArray.size());
	Texture1D_4I childsTexture(GLSLArray.size() * 2); //8 childs index (2*RGBA) per "texel"

	for (int i = 0; i < GLSLArray.size(); ++i) {

		Cell* currentCell = GLSLArray[i];

		colorTexture.set(i, currentCell->voxel.color);
		minTexture.set(i, currentCell->voxel.min);
		maxTexture.set(i, currentCell->voxel.max);
	}

	for (int i = 0, j = 0; i < GLSLArray.size() * 2; i += 2, j++) {

		Cell* currentCell = GLSLArray[j];

		childsTexture.set(i, glm::vec4(
			currentCell->childs[0],
			currentCell->childs[1],
			currentCell->childs[2],
			currentCell->childs[3]));

		childsTexture.set(i+1, glm::vec4(
			currentCell->childs[4],
			currentCell->childs[5],
			currentCell->childs[6],
			currentCell->childs[7]));

		currentCell = GLSLArray[j];
	}

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// EVENT
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		Do_Movement();

		compute->use();
		compute->setUniform1i("texSize", GLSLArray.size());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_output);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_1D, colorTexture.genGLTexture());
		compute->setUniform1i("colors", 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_1D, minTexture.genGLTexture());
		compute->setUniform1i("mins", 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_1D, maxTexture.genGLTexture());
		compute->setUniform1i("maxs", 2);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_1D, childsTexture.genGLTexture());
		compute->setUniform1i("childs", 3);


		// RENDERING
		camera->updateRayTrace();

		compute->use();
		glDispatchCompute(screenWidth, screenHeight, 1); // 512^2 threads in blocks of 16^2

		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		// draw
		renderer.renderToQuad(tex_output);

		// Swap the buffers
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose

	glfwTerminate();
	return 0;
}

// Moves/alters the camera positions based on user input
void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera->processMove(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera->processMove(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera->processMove(LEFTWARD, deltaTime);
	if (keys[GLFW_KEY_D])
		camera->processMove(RIGHTWARD, deltaTime);
}

void wireframe(DefaultRenderer * renderer)
{
	// Camera controls
	if (keys[GLFW_KEY_F])
		renderer->setWireframe(false);
	if (keys[GLFW_KEY_Z])
		renderer->setWireframe(true);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	camera->processLook(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->processZoom(yoffset / 100.f);
}