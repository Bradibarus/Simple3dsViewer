#pragma once
#include <vector>
#include <glm1\glm.hpp>
#include "Shader.h"

struct Vertex {
	glm::vec3 Position;
	//glm::vec2 TexCoords;
};

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	Mesh() {};
	void draw(Shader &shader);
	void setup();
private:
	GLuint VAO, VBO, EBO;
};