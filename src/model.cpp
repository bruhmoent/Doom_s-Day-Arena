#include "model.h"

glm::mat4 Model::getModelMatrix() 
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);
    return model;
}

void Model::setDepthFromCamera(glm::vec3 cameraPos, glm::vec3 modelPos){ m_depthFromCamera = glm::length(cameraPos - modelPos); }

glm::mat4 Model::getRotationMatrix(float xRotation, float yRotation) {
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.3, 0.3));
    modelMatrix = glm::rotate(modelMatrix, xRotation, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, yRotation, glm::vec3(0.0f, 1.0f, 0.0f));
    return modelMatrix;
}

void Model::bindVertexData() {
    if (m_vbo == 0) {
        glGenBuffers(1, &m_vbo);
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);
}

GLuint Model::loadTexture(const std::string& textureFilePath) {
    sf::Image image;
    if (!image.loadFromFile(textureFilePath)) {
        std::cerr << "Failed to load texture: " << textureFilePath << std::endl;
        return 0;
    }

    GLuint textureId;
    glGenTextures(1, &textureId);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "Failed to generate texture: " << error << std::endl;
        return 0;
    }

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format;
    if (image.getSize().x > 0) {
        format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, image.getSize().x, image.getSize().y, 0, format, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    }
    else {
        std::cerr << "Unknown texture format" << std::endl;
        return 0;
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    return textureId;
}

void Model::setVertexData(const std::vector<float>& vertexData)
{
    m_vertices = vertexData;
}

glm::vec3 Model::getPosition() { return position; }

glm::vec3 Model::getScale() { return scale; }

glm::vec3 Model::getRotation() { return rotation; }

std::vector<unsigned int> Model::getIndices() const { return m_indices; }

std::vector<float> Model::getVertexData() const{ return m_vertices; }