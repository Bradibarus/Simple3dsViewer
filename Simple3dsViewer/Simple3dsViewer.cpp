#include <iostream>
#include <fstream>
#include <vector>
#include "glad/glad.h"
#include "glfw/include/GLFW/glfw3.h"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Mesh.h"
#include "3DSloader.h"

int WIDTH = 800;
int HEIGHT = 600;
bool keys[1024]; //array for key callbacks
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLfloat zoom = -250;
GLfloat moveY = 0.0f;
GLfloat moveX = 0.0f;
bool wireframeOn = 0;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void update(Shader& shader);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main(int argc, char *argv[])
{
	char* filePath = new char[100];

	if (argc < 2) {
		std::cout << "Enter file path to the destination .3ds file (you can also enter it as a command line argument): " << std::endl;
		std::cin >> filePath;
	}
	else {
		filePath = argv[1];
	}
	
	//	Attempt to load needed data from specified file. On failure program exits:

	Model* model = new Model();
	Loader3DS* loader = new Loader3DS(model, filePath);
	if (loader->load() == 0) {
		std::cout << "LOADING SUCCESSFUL" << std::endl;
	}
	else {
		std::cout << "ERROR LOADING FROM FILE" << std::endl;
		return -1;
	}

	//	Basic glfw initialization:

	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "3DS File Viewer", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetKeyCallback(window, key_callback);


	model->setup();
	Shader* shader = new Shader("Vertex.shader", "Fragment.shader");

	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		if (keys[GLFW_KEY_ESCAPE]) glfwSetWindowShouldClose(window, GL_TRUE);

		update(*shader);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		model->draw(*shader);
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	WIDTH = width;
	HEIGHT = height;
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void update(Shader& shader)
{
	static bool wasPressed = 0;
	if (keys[GLFW_KEY_UP]) moveY -= 100 * deltaTime;
	if (keys[GLFW_KEY_DOWN]) moveY += 100 * deltaTime;
	if (keys[GLFW_KEY_W]) zoom += 100 * deltaTime;
	if (keys[GLFW_KEY_S]) zoom -= 100 * deltaTime;
	if (keys[GLFW_KEY_P] && !wasPressed) {
		if (!wireframeOn) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			wireframeOn = 1;
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			wireframeOn = 0;
		}
		wasPressed = 1;
	};
	if (!keys[GLFW_KEY_P]) wasPressed = 0;

	glm::mat4 model;
	glm::mat4 projection;
	model = glm::translate(model, glm::vec3(moveX, moveY, zoom));
	model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.f, 1.0f, 0.0f));
	projection = glm::perspective(glm::radians(45.0f), (GLfloat) WIDTH / (GLfloat)HEIGHT, 0.1f, 20000.0f);
	GLint modelLoc = glGetUniformLocation(shader.Program, "model");
	GLint projLoc = glGetUniformLocation(shader.Program, "projection");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}