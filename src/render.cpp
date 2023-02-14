#include "render.h"
#include "shader.h"
#include "loadObject.h"
#include "model.h"
#include "camera.h"
#include "runtime.h"

extern unsigned int g_VBO, g_EBO, g_VAO;
unsigned int g_textureID;

void clearBuffer(int width, int height)
{
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void RenderEngine::renderScene(Shader& shader, Model& model, Camera& camera, GLFWwindow*& window)
{

	glfwPollEvents();

	shader.use();

	int success;
	char infoLog[512];
	glGetProgramiv(shader.m_shaderID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader.m_shaderID, 512, NULL, infoLog);
		std::cerr << "Error linking shader program: " << infoLog << std::endl;
		return;
	}

	model.bindVertexData();	

	glActiveTexture(GL_TEXTURE0 + 2);
	GLint texture_diffuse = glGetUniformLocation(shader.m_shaderID, "texture2");
	glUniform1i(texture_diffuse, 2);
	glBindTexture(GL_TEXTURE_2D, model.m_textures["test"]);

	glGenBuffers(1, &g_VBO);
	glGenBuffers(1, &g_EBO);

	glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
	glBufferData(GL_ARRAY_BUFFER, model.m_vertices.size() * sizeof(float), model.m_vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.m_indices.size() * sizeof(unsigned int), model.m_indices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, model.m_vertices.size() * sizeof(float), model.m_vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glDrawElements(GL_TRIANGLES, model.m_indices.size(), GL_UNSIGNED_INT, 0);
	glEnable(GL_DEPTH_TEST);

	glm::mat4 rotation;
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.3, 0.3));
	glm::mat4 view = glm::lookAt(glm::vec3(1.0f, 0.0f, 3.0f), glm::vec3(-0.1f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	float fov = 45.0f;
	float aspect = (float)800 / (float)600;
	float nearPlane = 0.1f;
	float farPlane = 100.0f;
	glm::mat4 projection = glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	float maxAnglePerFrame = 0.001f;
	float limitAngle = 0.002f;
	float dampingFactor = 0.0001f;
	float xRotation = 0.0f;
	float yRotation = 0.0f;

	glUseProgram(shader.m_shaderID);
	glValidateProgram(shader.m_shaderID);
	modelMatrix = glm::rotate(modelMatrix, xRotation, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, yRotation, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.m_shaderID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader.m_shaderID, "view"), 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	camera.processMouseMovement(xpos, ypos, true);
	rotation = glm::rotate(glm::mat4(1.0f), xRotation, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), yRotation, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.m_shaderID, "rotation"), 1, GL_FALSE, glm::value_ptr(rotation));

	glUniform3f(glGetUniformLocation(shader.m_shaderID, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	glUniform3f(glGetUniformLocation(shader.m_shaderID, "cameraFront"), camera.Front.x, camera.Front.y, camera.Front.z);
	glUniformMatrix4fv(glGetUniformLocation(shader.m_shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	clearBuffer(800, 600);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.m_indices.size() * sizeof(unsigned int), model.m_indices.data(), GL_STATIC_DRAW);
	glBindVertexArray(g_VAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glDrawElements(GL_TRIANGLES, model.m_indices.size(), GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(window);
	glfwPollEvents();

}