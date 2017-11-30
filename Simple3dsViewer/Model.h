/*
Just kind of a container for meshes
*/

#pragma once
#include <vector>
#include "Mesh.h"

class Model
{
public:
	Model() {};
	std::vector<Mesh> meshes;
	void setup();
	void draw(Shader& shader);
};