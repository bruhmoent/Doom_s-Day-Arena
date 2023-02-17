#include "camera.h"
std::unique_ptr<Camera> g_camera;

void Camera::processKeyboardInput(GLFWwindow* window, bool& moveForward, bool& moveBackward, bool& moveLeft, bool& moveRight)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		moveForward = true;
	else
		moveForward = false;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		moveBackward = true;
	else
		moveBackward = false;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		moveLeft = true;
	else
		moveLeft = false;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		moveRight = true;
	else
		moveRight = false;
}


glm::mat4 Camera::getViewMatrix() { return glm::lookAt(Position, Position + Front, Up);}

void Camera::resetCamera() { g_camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)); }

glm::vec3 Camera::getFront() { return Front; }

glm::vec3 Camera::getUp() { return Up; }

float Camera::getZoom() { return zoom; }

void Camera::setZoom(float value) { zoom = value; }

void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

	static float lastX = 800 / 2.0f;
	static float lastY = 600 / 2.0f;
	static bool firstMouse = true;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	g_camera->processMouseMovement(lastX, lastY, true);

}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	glm::vec3 front = getFront();
	front.x = std::cos(glm::radians(yoffset)) * std::cos(glm::radians(xoffset));
	front.y = std::sin(glm::radians(yoffset));
	front.z = std::cos(glm::radians(yoffset)) * std::sin(glm::radians(xoffset));
	front = glm::normalize(front);
	setFront2(front);
}

glm::vec3 Camera::getPosition() { return Position; }

glm::vec3 Camera::getRotation() { return Front; }

void Camera::setFront2(const glm::vec3& position) {
	g_camera->Front;
	Front = position;
}

