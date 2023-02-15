#include "loadObject.h"
#include "runtime.h"
#include "shader.h"
#include "camera.h"
#include "render.h"
#include "model.h"
#include <C:\Users\bar12\source\repos\openGL\openGL\glad.c>

extern std::vector<Model> g_models;
Shader g_shader;
Model g_model;
GLFWwindow* g_window;
RenderEngine g_renderEngine;
ModelLoader g_load;

void runTimeProcessx64::initGL(GLFWwindow*& window)
{
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW." << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(800, 600, "Doom's Day : Arena", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	sf::Image icon;
	if (!icon.loadFromFile("icon.png")) {
		std::cerr << "Failed to load icon image." << std::endl;
		exit(EXIT_FAILURE);
	}

	GLFWimage glfwIcon;
	glfwIcon.width = icon.getSize().x;
	glfwIcon.height = icon.getSize().y;
	glfwIcon.pixels = new unsigned char[glfwIcon.width * glfwIcon.height * 4];
	for (int y = 0; y < glfwIcon.height; ++y) {
		for (int x = 0; x < glfwIcon.width; ++x) {
			sf::Color color = icon.getPixel(x, y);
			glfwIcon.pixels[(y * glfwIcon.width + x) * 4 + 0] = color.r;
			glfwIcon.pixels[(y * glfwIcon.width + x) * 4 + 1] = color.g;
			glfwIcon.pixels[(y * glfwIcon.width + x) * 4 + 2] = color.b;
			glfwIcon.pixels[(y * glfwIcon.width + x) * 4 + 3] = color.a;
		}
	}

	glfwSetWindowIcon(window, 1, &glfwIcon);

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD." << std::endl;
		exit(EXIT_FAILURE);
	}

	delete[] glfwIcon.pixels;
	glViewport(0, 0, 800, 600);
	glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(g_window, Camera::mouse_callback);
}

void runTimeProcessx64::RUN()
{

	g_camera.reset(new Camera);
	std::vector<Model> models = g_load.loadModel("../assets/stages/baseplate.obj");
	g_shader = Shader("shader.vs", "shader.fs");
	g_camera->resetCamera();
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);  
	glDepthFunc(GL_LEQUAL);   
	glShadeModel(GL_SMOOTH);  
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();  
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(g_window))
	{

		glm::vec3 cameraPosition = g_camera->getFront();
		glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CROSSHAIR_CURSOR);
		glfwSetCursorPosCallback(g_window, Camera::mouse_callback);
		g_renderEngine.renderScene(g_shader, models, *g_camera, g_window);

	}

}