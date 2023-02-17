#include "runtime.h"
runTimeProcessx64 runtime;
extern GLFWwindow* g_window;
int main()
{

	runtime.initGL(g_window);
	runtime.RUN();

	glfwTerminate();
	glfwDestroyWindow(g_window);
	return 0;

}