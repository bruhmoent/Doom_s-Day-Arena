#pragma once
#include "definition.h"

#ifndef RENDER_H
#define RENDER_H

class Shader;
class Camera;
class Model;

class RenderEngine
{

public:
	void renderScene(Shader& shader, std::vector<Model>& models, Camera& camera, GLFWwindow*& window);

};

#endif