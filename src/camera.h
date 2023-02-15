#pragma once
#include "definition.h"

#ifndef CAMERA_H
#define CAMERA_H

class Camera {

public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) : Position(position),
		Up(up) {
		Front = front;
	}

	glm::mat4 getViewMatrix();

	glm::vec3 getPosition();

	glm::vec3 getFront();

	glm::vec3 getUp();

	void resetCamera();

	glm::vec3 getRotation();

	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch);

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	void setFront2(const glm::vec3& position);

	float getZoom();

	void setZoom(float value);

	float zoom = 1.0f;

};

extern std::unique_ptr<Camera> g_camera;

#endif