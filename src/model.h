#pragma once
#include "definition.h"
#include "camera.h"

#ifndef MODEL_H
#define MODEL_H

class Model {

public:
	Model() : m_vbo(0) {}
	~Model() {
		if (m_vbo != 0) {
			glDeleteBuffers(1, &m_vbo);
		}
		for (auto& texture : m_textures) {
			glDeleteTextures(1, &texture.second);
		}
	}

	std::vector<float> m_vertices;
	std::vector<unsigned int> m_indices;
	float m_depthFromCamera;
	std::map<std::string, unsigned int> m_textures;

	GLuint loadTexture(const std::string& textureFilePath);

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);
	glm::vec3 rotation = glm::vec3(0.0f);

	void setVertexData(const std::vector<float>& vertexData);

	glm::mat4 getRotationMatrix(float xRotation, float yRotation);

	void setDepthFromCamera(glm::vec3 cameraPos, glm::vec3 modelPos);

	void loadTexture(const std::string& textureFilePath, const std::string& textureName);

	void bindVertexData();

	glm::vec3 getPosition();

	glm::mat4 getModelMatrix();

	glm::vec3 getScale();

	glm::vec3 getRotation();

	std::vector<unsigned int> getIndices() const;

	std::vector<float> getVertexData() const;

private:
	GLuint m_vbo;

};

#endif