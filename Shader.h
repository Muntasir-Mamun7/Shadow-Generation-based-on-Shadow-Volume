#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef void(*CheckStatus)(GLuint, GLenum, GLint*);
typedef void(*ReadLog)(GLuint, GLsizei, GLsizei*, GLchar*);
#define CheckErrorAndLog(CheckStatus, ReadLog) CheckStatus(obj, checkStatus, &success);if(!success){GLchar infoLog[1024];ReadLog(obj, 1024, NULL, infoLog); std::cout << "ERROR::SHADER::FAILED\n"<< std::endl; exit(-1);}

class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		shaderProgram = glCreateProgram();
		CompileShaderFromFile(vertexPath, GL_VERTEX_SHADER);
		CompileShaderFromFile(fragmentPath, GL_FRAGMENT_SHADER);
		LinkShaderProgram();
	}
	~Shader()
	{
		glDeleteProgram(shaderProgram);
	}
	void UseProgram()
	{
		glUseProgram(shaderProgram);
	}
	void SetMatrix4fv(const std::string& name, glm::mat4 matrix)const
	{
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}
	void SetVec3(const std::string& name, glm::vec3 pos)const
	{
		glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), pos.x, pos.y, pos.z);
	}
	// Add this new function
	void SetFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
	}
private:
	GLuint shaderProgram;
	void CompileShaderFromFile(const char* path, GLenum shaderType)
	{
		// Read from file path
		std::string code;
		std::ifstream shaderFile;
		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			shaderFile.open(path);
			std::stringstream shaderStream;
			shaderStream << shaderFile.rdbuf();
			shaderFile.close();
			code = shaderStream.str();
		}
		catch (std::istringstream::failure& e) {
			std::cout << "ERROR::SHADER::FILE_NOT_READ: " << e.what() << std::endl;
		}
		// compile the shade code
		const GLchar* shaderCode = code.c_str();
		GLuint shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &shaderCode, NULL);
		glCompileShader(shader);
		CheckErrors(shader, "SHADER", GL_COMPILE_STATUS);
		glAttachShader(shaderProgram, shader);
		glDeleteShader(shader);
	}
	void LinkShaderProgram()
	{
		glLinkProgram(shaderProgram);
		CheckErrors(shaderProgram, "PROGRAM", GL_LINK_STATUS);
#ifdef __WIN64
		glValidateProgram(shaderProgram);
		CheckErrors(shaderProgram, "PROGRAM", GL_VALIDATE_STATUS);
#endif
	}
	void CheckErrors(GLuint obj, std::string type, int checkStatus)
	{
		GLint success;
		if (type == "SHADER") {
			CheckErrorAndLog(glGetShaderiv, glGetShaderInfoLog);
		}
		else if (type == "PROGRAM") {
			CheckErrorAndLog(glGetProgramiv, glGetProgramInfoLog);
		}
	}
};