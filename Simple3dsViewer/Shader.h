/*
Encapsulation of shaders compilation, linking and use
*/

#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "glad/glad.h"

class Shader {
public:
	GLuint Program;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void Use();
	~Shader();
};
