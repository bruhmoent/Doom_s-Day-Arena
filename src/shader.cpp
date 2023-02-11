#include "shader.h"

void Shader::checkShaderSourceErrors(GLuint shader, const char* type) {
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
	}
}

unsigned int Shader::getID() {
	return m_shaderID;
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

void Shader::use() const { glUseProgram(m_shaderID); }

void Shader::setBool(const std::string& name, bool value) const { glUniform1i(glGetUniformLocation(m_shaderID, name.c_str()), (int)value); }

void Shader::setInt(const std::string& name, int value) const { glUniform1i(glGetUniformLocation(m_shaderID, name.c_str()), value); }

void Shader::setFloat(const std::string& name, float value) const { glUniform1f(glGetUniformLocation(m_shaderID, name.c_str()), value); }

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const { glUniformMatrix4fv(glGetUniformLocation(m_shaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]); }