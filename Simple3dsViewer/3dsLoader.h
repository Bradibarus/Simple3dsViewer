#pragma once

#include <stdio.h>
#include <io.h>
#include <string>
#include "glm1\glm.hpp"
#include "Mesh.h"
#include "Model.h"

#define PRIMARY       0x4D4D

#define OBJECTINFO    0x3D3D

#define OBJECT		  0x4000

#define OBJECT_MESH   0x4100

#define OBJECT_VERTICES     0x4110	
#define OBJECT_FACES		0x4120	
#define OBJECT_UV			0x4140			

struct Chunk
{
	unsigned short int id;
	unsigned int length;
	unsigned int bytesRead;
};

class  Loader3DS
{
public:
	Loader3DS(Model* model, const char* filePath);
private:
	FILE* filePointer;
	Chunk* currentChunk;
	void readChunk(Chunk* chunk);
	void processNextChunk(Model* model, Chunk* chunk);

	void readChars(Chunk* chunk);
	void readVertices(Model* model, Chunk* chunk);
	void readIndices(Model* model, Chunk* chunk);

};