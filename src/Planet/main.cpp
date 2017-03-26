#pragma once

// Std. Includes
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Planet.h"
#include "Shaders.h"
#include "OrbitalManipulator.h"
#include "Camera.h"
#include "MVP.h"
#include "DirectionalLight.h"
#include "Mesh.h"
#include "DisplacementPhongMaterial.h"
#include "PhongMaterial.h"
#include "SelfIlluminMaterial.h"
#include "CubeMap.h"
#include "HeightMapFilter.h"
#include "Noise.h"
#include "Scene.h"

#include "DefaultRenderer.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#define NBCHUNK 1

// Properties
GLuint screenWidth = 1600, screenHeight = 900;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
void wireframe(DefaultRenderer * renderer);
void stopSelection();

// Camera
Camera* camera = new Camera((float)screenWidth, (float)screenHeight, 0.01f, 1000.f, 0.90f);

bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
bool stopSelect = false;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

unsigned int circleSpeed = 1;

// The MAIN function, from here we start our application and run our Game loop
int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "VArch - Octree", nullptr, nullptr);
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

	// Camera
	camera->transform.setPosition(glm::vec3(16.3f, 10.7f, -6.5f));
	camera->transform.setRotation(glm::vec3(-40.f, 124.f, 0.f));

	OrbitalManipulator* orbitalManipulator = new OrbitalManipulator(camera);
	camera->setManipulator(orbitalManipulator);

	// Set material textures
	PhongMaterial *mat = new PhongMaterial();
	mat->ambient = glm::vec3(1);
	mat->diffuse = glm::vec3(1);
	mat->specular = glm::vec3(1);
	mat->shininess = 32;

	SelfIlluminMaterial *SIMat = new SelfIlluminMaterial();

	// HeightMap
	NoiseProperties np = NoiseProperties(0.4, 0.5, 8);
	CubeMap cubeMap(256);
	cubeMap.generateSimplex(np);
	cubeMap.transformInterval(-0.5f, -0.2f);

	// Light
	DirectionalLight* light = new DirectionalLight;
	light->direction = glm::vec3(0.f, -1, 0.f);
	light->diffuse = glm::vec3(1);
	//light->transform.translate(glm::vec3(0, 2, 0));

	Mesh* refCube = new Mesh(mat);
	Mesh* landMarkMesh = new Mesh(SIMat);
	landMarkMesh->generateLandmark();

	// Planet
	ColorTable colorTable = ColorTable::Nature(128);
	Planet planet(cubeMap, colorTable);

	Object *root = new Object();
	planet.addMeshesToObject(root);

	Object *landMark = new Object();
	landMark->addComponent(landMarkMesh);

	//landMark->transform.translate(glm::vec3(-1, -1, -1));

	root->addChild(landMark);

	// Scene
	Scene scene = Scene(root, camera);
	scene.addLight(light);

	DefaultRenderer renderer(&scene);
	renderer.setWireframe(false);

	Shaders::getInstance()->useShader(BuiltInShader::PHONG);

	int demulti = 0;

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		Do_Movement();
		wireframe(&renderer);
		stopSelection();

		//light->direction = camera->front();

		if (!stopSelect) {

			demulti = 0;
		}

		demulti++;

		renderer.render();
		//renderer.renderToQuad(terrain.colorMap()->genGLTexture());

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
	float speed = 1.f;

	// Camera controls
	if (keys[GLFW_KEY_W])
		camera->processMove(CameraMovement::FORWARD, deltaTime / speed);
	if (keys[GLFW_KEY_S])
		camera->processMove(CameraMovement::BACKWARD, deltaTime / speed);
	if (keys[GLFW_KEY_A])
		camera->processMove(CameraMovement::LEFTWARD, deltaTime / speed);
	if (keys[GLFW_KEY_D])
		camera->processMove(CameraMovement::RIGHTWARD, deltaTime / speed);

}

void stopSelection()
{
	// Camera controls
	if (keys[GLFW_KEY_H])
		stopSelect = false;
	if (keys[GLFW_KEY_G])
		stopSelect = true;
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