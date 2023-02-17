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

    std::vector<Model> loadModel(const std::string& path);

    std::vector<float> m_vertexData;

};

#endif