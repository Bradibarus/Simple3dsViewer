#include "Model.h"

void Model::draw(Shader& shader) {
	for (int i = 0; i < this->meshes.size(); i++) {
		this->meshes[i].draw(shader);
	}
}

void Model::setup() {
	for (int i = 0; i < this->meshes.size(); i++) {
		this->meshes[i].setup();
	}
}