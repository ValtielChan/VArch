#pragma once

// Std. Includes
#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "VoxelGrid.h"
#include "Shaders.h"
#include "Camera.h"
#include "MVP.h"
#include "DirectionalLight.h"
#include "Mesh.h"
#include "DisplacementPhongMaterial.h"
#include "PhongMaterial.h"
#include "CubeMap.h"
#include "HeightMapFilter.h"

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
Camera* camera = new Camera((float)screenWidth, (float)screenHeight, 0.1f, 1000.f, 0.45f);
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


	// Set material textures
	PhongMaterial *mat = new PhongMaterial();

	// Light
	DirectionalLight* light = new DirectionalLight;
	light->direction = glm::vec3(1, -0.5f, 0.7f);
	//light->transform.translate(glm::vec3(0, 2, 0));

	// Objects
	VoxelGrid voxelGrid = VoxelGrid(16, 3, 16, .1f);
	voxelGrid.fill();
	voxelGrid.buildMesh();
	voxelGrid.mesh()->setMaterial(mat);
	
	Mesh* cube = new Mesh(mat);

	Object *root = new Object();
	root->addComponent(voxelGrid.mesh());
	//root->addComponent(cube);

	// Scene
	Scene scene = Scene(root, camera);
	scene.addLight(light);

	DefaultRenderer renderer(&scene);
	renderer.setWireframe(false);

	Shaders::getInstance()->useShader(BuiltInShader::DISPLACEMENT);

	// Test cubemap
	CubeMap cb(128);
	cb.generateSimplex(1, 1, 4);
	HeightMap* hm = cb.GetUniqueHeightMap();
	//HeightMap* hm = new HeightMap(128, 128);
	//hm->generateSimplex(new NoiseProperties(1, 1, 4));
	//hm->transformInterval(0, 1);
	HeightMapFilter::binarize(hm, 0);
	GLuint cubemap = hm->genGLTexture();

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

		//renderer.render();
		renderer.renderToQuad(cubemap);

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
		camera->processMove(CameraMovement::FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera->processMove(CameraMovement::BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera->processMove(CameraMovement::LEFTWARD, deltaTime);
	if (keys[GLFW_KEY_D])
		camera->processMove(CameraMovement::RIGHTWARD, deltaTime);

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