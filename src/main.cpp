#include "runtime.h"
runTimeProcessx64 runtime;
extern GLFWwindow* g_window;

int main()
{
	runtime.initGL(g_window);
	runtime.RUN();
	glfwTerminate();
	return 0;
}