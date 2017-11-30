#pragma once
#include "Shader.h"

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	std::string vShaderString;
	std::string fShaderString;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.open(vertexPath);
	fShaderFile.open(fragmentPath);

	std::stringstream vShaderStream;
	std::stringstream fShaderStream;

	vShaderStream << vShaderFile.rdbuf();
	fShaderStream << fShaderFile.rdbuf();

	vShaderString = vShaderStream.str();
	fShaderString = fShaderStream.str();

	const GLchar* vShaderChars = vShaderString.c_str();
	const GLchar* fShaderChars = fShaderString.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	this->Program = glCreateProgram();

	glShaderSource(vertexShader, 1, &vShaderChars, nullptr);
	glShaderSource(fragmentShader, 1, &fShaderChars, nullptr);

	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infolog[512];
		glGetShaderInfoLog(vertexShader, 512, nullptr, infolog);
		std::cout << "VERTEX SHADER COMPILATION FAILED\n" << infolog << std::endl;
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infolog[512];
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infolog);
		std::cout << "FRAGMENT SHADER COMPILATION FAILED\n" << infolog << std::endl;
	}

	glAttachShader(this->Program, vertexShader);
	glAttachShader(this->Program, fragmentShader);
	glLinkProgram(this->Program);

	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar infolog[512];
		glGetProgramInfoLog(this->Program, 512, nullptr, infolog);
		std::cout << "SHADER PROGRAM LINKING FAILED\n" << infolog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Use() {
	glUseProgram(this->Program);
}

Shader::~Shader() {
	glDeleteProgram(this->Program);
}