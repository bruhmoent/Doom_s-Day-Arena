#pragma once
#include "definition.h"

#ifndef SHADER_H
#define SHADER_H

class Shader
{

public:
	unsigned int m_shaderID;
	Shader() = default;

	void checkCompileErrors(unsigned int shader, std::string type);

	void checkShaderSourceErrors(GLuint shader, const char* type);

	Shader(const char* vertexPath, const char* fragmentPath)
	{

		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{

			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			vShaderFile.close();
			fShaderFile.close();
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();

		}

		catch (std::ifstream::failure e){ std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl; }

		int vertexCodeLength = vertexCode.length();
		int fragmentCodeLength = fragmentCode.length();
		std::vector<char> vShaderCode(vertexCode.c_str(), vertexCode.c_str() + vertexCode.size() + 1);
		std::vector<char> fShaderCode(fragmentCode.c_str(), fragmentCode.c_str() + fragmentCode.size() + 1);
		const char* vShaderCodePtr = vShaderCode.data();
		const char* fShaderCodePtr = fShaderCode.data();
		GLuint vertex, fragment;
		int success;
		char infoLog[512];

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCodePtr, NULL);
		glCompileShader(vertex);

		checkCompileErrors(vertex, "VERTEX");
		checkShaderSourceErrors(vertex, "VERTEX");

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCodePtr, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		checkShaderSourceErrors(fragment, "FRAGMENT");

		m_shaderID = glCreateProgram();
		glAttachShader(m_shaderID, vertex);
		glAttachShader(m_shaderID, fragment);
		glLinkProgram(m_shaderID);
		checkCompileErrors(m_shaderID, "PROGRAM");

		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}

	void use() const;

	void setBool(const std::string& name, bool value) const;

	void setInt(const std::string& name, int value) const;

	void setFloat(const std::string& name, float value) const;

	unsigned int getID();

	void setMat4(const std::string& name, const glm::mat4& mat) const;

	void setVec3(const std::string& name, const glm::vec3& value) const;

};

#endif