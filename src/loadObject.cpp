#include "definition.h"
#include "model.h"
#include "loadObject.h"
#include "shader.h"
unsigned int g_VBO, g_EBO, g_VAO;
std::vector<Model> g_models;

void ModelLoader::clearBuffer(int width, int height)
{
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

std::vector<Model> ModelLoader::loadModel(const std::string& path)
{
    std::vector<Model> models;
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error loading model: " << importer.GetErrorString() << std::endl;
        return models;
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];
        Model model;
        glm::vec3 l_position = glm::vec3(mesh->mVertices[0].x, mesh->mVertices[0].y, mesh->mVertices[0].z);
        glm::vec3 min = glm::vec3(FLT_MAX);
        glm::vec3 max = glm::vec3(-FLT_MAX);

        for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
            aiVector3D pos = mesh->mVertices[j];
            m_vertexData.push_back(pos.x);
            m_vertexData.push_back(pos.y);
            m_vertexData.push_back(pos.z);
            if (pos.x < min.x) min.x = pos.x;
            if (pos.y < min.y) min.y = pos.y;
            if (pos.z < min.z) min.z = pos.z;
            if (pos.x > max.x) max.x = pos.x;
            if (pos.y > max.y) max.y = pos.y;
            if (pos.z > max.z) max.z = pos.z;

            if (mesh->HasNormals()) {
                aiVector3D normal = mesh->mNormals[j];
                m_vertexData.push_back(normal.x);
                m_vertexData.push_back(normal.y);
                m_vertexData.push_back(normal.z);
            }

            if (mesh->HasTextureCoords(0)) {
                aiVector3D uv = mesh->mTextureCoords[0][j];
                m_vertexData.push_back(uv.x);
                m_vertexData.push_back(uv.y);
            }
        }

        glm::vec3 center = (min + max) * 0.5f;
        glm::vec3 extents = max - center;
        float radius = glm::length(extents);
        model.setVertexData(m_vertexData);

        aiNode* node = scene->mRootNode->FindNode(mesh->mName);
        aiMatrix4x4 transform = node->mTransformation;
        transform.Transpose();
        glm::mat4 modelMatrix = glm::make_mat4(&transform.a1);

        for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
            aiFace face = mesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; k++) {
                model.m_indices.push_back(face.mIndices[k]);
            }
        }
        //TO DO: Fix scaling and position data.
        glm::vec3 l_scale = glm::vec3(extents.x, extents.y, extents.z);
        models.push_back(model);
        models[i].position = l_position;
        models[i].scale = l_scale;
    }
    return models;
}