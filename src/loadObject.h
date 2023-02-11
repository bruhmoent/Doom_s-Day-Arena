#pragma once
#include "definition.h"

#ifndef LOAD_OBJECT_H
#define LOAD_OBJECT_H

class Shader;
class Model;

class ModelLoader {
public:

    ModelLoader() = default;
    ~ModelLoader() = default;

    Model loadModel(const std::string& path);

    void clearBuffer(int width, int height);

    std::vector<float> m_vertexData;

};
#endif

