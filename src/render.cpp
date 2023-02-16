#include "render.h"
#include "shader.h"
#include "loadObject.h"
#include "model.h"
#include "camera.h"
#include "runtime.h"

void clearBuffer(int width, int height)
{
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}


void RenderEngine::renderScene(Shader& shader, std::vector<Model>& models, Camera& camera, GLFWwindow*& window)
{

	glfwPollEvents();

	shader.use();

	std::vector<GLuint> vbos(models.size());
	std::vector<GLuint> vaos(models.size());
	std::vector<GLuint> ebos(models.size());

	int success;
	char infoLog[512];
	glGetProgramiv(shader.m_shaderID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader.m_shaderID, 512, NULL, infoLog);
		std::cerr << "Error linking shader program: " << infoLog << std::endl;
		return;
	}

	glm::mat4 view = glm::lookAt(glm::vec3(1.0f, 0.0f, 3.0f), glm::vec3(-0.1f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	float fov = 45.0f;
	float aspect = (float)800 / (float)600;
	float nearPlane = 0.1f;
	float farPlane = 100.0f;
	bool moveForward = false;
	bool moveBackward = false;
	bool moveLeft = false;
	bool moveRight = false;
	float cameraSpeed = 0.25f;
	float xRotation = 0.0f;
	float yRotation = 0.0f;
	glm::mat4 projection = glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);

	glUseProgram(shader.m_shaderID);
	glValidateProgram(shader.m_shaderID);

	for (int i = 0; i < models.size(); i++) {
		models[i].setDepthFromCamera(camera.getPosition(), models[i].getPosition());
	}

	std::sort(models.begin(), models.end(), [](const Model& a, const Model& b) {
		return a.m_depthFromCamera > b.m_depthFromCamera;
		});

	for (int i = 0; i < models.size(); i++) {
		models[i].loadTexture("../assets/images/floor3.png", "test");
		glActiveTexture(GL_TEXTURE0 + 2);
		GLint texture_diffuse = glGetUniformLocation(shader.m_shaderID, "texture2");
		glUniform1i(texture_diffuse, 2);
		glBindTexture(GL_TEXTURE_2D, models[i].m_textures["test"]);
	}

	clearBuffer(800, 600);

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.3, 0.3));
	modelMatrix = glm::rotate(modelMatrix, xRotation, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, yRotation, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.m_shaderID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glm::mat4 modelRotation = glm::rotate(glm::mat4(1.0f), xRotation, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), yRotation, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.m_shaderID, "rotation"), 1, GL_FALSE, glm::value_ptr(modelRotation));
	for (int i = 0; i < models.size(); ++i)
	{
		glGenVertexArrays(1, &vaos[i]);
		glBindVertexArray(vaos[i]);
		glGenBuffers(1, &vbos[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vbos[i]);
		glGenBuffers(1, &ebos[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos[i]);

		glBufferData(GL_ARRAY_BUFFER, models[i].m_vertices.size() * sizeof(float), models[i].m_vertices.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, models[i].m_indices.size() * sizeof(unsigned int), models[i].m_indices.data(), GL_STATIC_DRAW);
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, models[i].getPosition());
		modelMatrix = glm::scale(modelMatrix, models[i].scale);
		modelMatrix = glm::rotate(modelMatrix, xRotation, glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, yRotation, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.m_shaderID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(shader.m_shaderID, "rotation"), 1, GL_FALSE, glm::value_ptr(modelRotation));

		glDrawElements(GL_TRIANGLES, models[i].m_indices.size(), GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	glUniformMatrix4fv(glGetUniformLocation(shader.m_shaderID, "view"), 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	camera.processMouseMovement(xpos, ypos, true);
	camera.processKeyboardInput(window, moveForward, moveBackward, moveLeft, moveRight);

	if (moveForward)
		camera.Position += cameraSpeed * camera.getFront();
	if (moveBackward)
		camera.Position -= cameraSpeed * camera.getFront();
	if (moveLeft)
		camera.Position -= glm::normalize(glm::cross(camera.getFront(), camera.getUp())) * cameraSpeed;
	if (moveRight)
		camera.Position += glm::normalize(glm::cross(camera.getFront(), camera.getUp())) * cameraSpeed;

	glUniform3f(glGetUniformLocation(shader.m_shaderID, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	glUniform3f(glGetUniformLocation(shader.m_shaderID, "cameraFront"), camera.Front.x, camera.Front.y, camera.Front.z);
	glUniformMatrix4fv(glGetUniformLocation(shader.m_shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glfwSwapBuffers(window);
	glfwPollEvents();

}