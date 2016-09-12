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
void wireframe(DefaultRenderer * renderer);

// Camera
Camera* camera = new Camera((float)screenWidth, (float)screenHeight, 0.1f, 1000.f, 0.8f);
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

unsigned int circleSpeed = 1;

// The MAIN function, from here we start our application and run our Game loop
int main()
{
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "VArch", nullptr, nullptr);
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


	// Material Setup
	DisplacementPhongMaterial* mat = new DisplacementPhongMaterial;

	mat->ambient = glm::vec3(0.1, 0.5, 0.2);
	mat->diffuse = glm::vec3(0.1, 0.75, 0.2);
	mat->specular = glm::vec3(0);
	mat->shininess = 256;

	HeightMap* heightMap = new HeightMap(512, 512);
	NormalMap* normalMap;
	
	// Generate Noise & normalize range
	NoiseProperties np = NoiseProperties(2, 1, 4);
	heightMap->generateSimplex(&np);
	heightMap->transformInterval(0.f, 1.f);

	// Generate Normal Map
	normalMap = heightMap->generateNormalMap();

	// Set material textures
	mat->heightmap = heightMap;
	mat->normalMap = normalMap;

	DirectionalLight* light = new DirectionalLight;
	light->direction = glm::vec3(1, -.5f, 1);
	light->diffuse = glm::vec3(0, 0.5, 0);
	//light->transform.translate(glm::vec3(0, 2, 0));

	// QuadTree
	std::vector<Vertex> vertices;
	std::vector<GLuint> vertexArray;
	float step = 20.f;

	for (int z = 0; z < 3; z++) {
		for (int x = 0; x < 3; x++) {
			vertices.push_back(Vertex(glm::vec3(x * step, 0, z * step)));
			vertexArray.push_back(z * 3 + x);
		}
	}

	std::vector<GLuint> empty;
	Mesh * terrain = new Mesh(vertices, empty, mat);
	Object* terrainWrap = new Object;

	terrainWrap->addComponent(terrain);
	QTNode node(terrainWrap, vertexArray, terrain->getIndices(), terrain->getVertices(), glm::vec3(step, 0, step), step);

	terrainWrap->transform.translate(glm::vec3(-step, 0, -step));

	node.selection(glm::vec3(step, 0, step));
	node.buildTriangles();

	// Scene
	Scene scene = Scene(terrainWrap, camera);
	scene.addLight(light);

	DefaultRenderer renderer(&scene);
	renderer.setWireframe(false);

	Shaders::getInstance()->useShader(BuiltInShader::DISPLACEMENT);

	float time = 0;

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		time += 0.01f;

		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		Do_Movement();
		wireframe(&renderer);

		renderer.render();

		terrain->getIndices()->clear();
		node.resetSelection();
		// Auto position selection
		//node.selection(glm::vec3(cos(time * circleSpeed) * 3, 0, sin(time * circleSpeed) * 3));
		node.selection(glm::vec3(0, 0, 0));
		// From camera position selection
		//node.selection(camera->transform.position());
		node.buildTriangles();

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

	// LOD autocircle speed control
	if (keys[GLFW_KEY_H])
		circleSpeed ++;
	if (keys[GLFW_KEY_G])
		if (circleSpeed > 0) circleSpeed --;
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
	camera->processZoom(yoffset/100.f);
}