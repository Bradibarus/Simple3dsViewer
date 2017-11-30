#include "3dsLoader.h"


Loader3DS::Loader3DS(Model* model, const char* filePath)
{
	this->model = model;
	this->filePath = filePath;
};

int Loader3DS::load() {

	currentChunk = new Chunk;

	fopen_s(&filePointer, filePath, "rb");
	std::cout << "Opening file: " << filePath << std::endl;
	if (!filePointer) {
		std::cout << "ERROR OPENING 3DS FILE" << std::endl;
		return -1;
	}

	if (filePointer) std::cout << "FILE OPENED" << std::endl;

	readChunk(currentChunk);
	if (currentChunk->id != PRIMARY) {
		std::cout << "ERROR READING PRIMARY CHUNK" << std::endl;
		return -1;
	}
	else std::cout << "PRIMARY CHUNK found!" << std::endl;


	processNextChunk(model, currentChunk);
	return 0;
}

void Loader3DS::readChunk(Chunk* chunk)
{
	std::cout << "Processing" << std::endl;
	chunk->bytesRead = fread(&chunk->id, 1, 2, filePointer);
	chunk->bytesRead += fread(&chunk->length, 1, 4, filePointer);
	
	std::cout << "\nChunk ID: " << std::hex << chunk->id << std::endl;
}

void Loader3DS::processNextChunk(Model* model, Chunk* previousChunk)
{
	currentChunk = new Chunk;
	while (previousChunk->bytesRead < previousChunk->length)
	{
		readChunk(currentChunk);
		switch (currentChunk->id)
		{
		case OBJECTINFO:
			std::cout << "OBJECTINFO CHUNK found" << std::endl;
			processNextChunk(model, currentChunk);
			break;
		case OBJECT:
			std::cout << "\nOBJECT CHUNK found" << std::endl;
			readChars(currentChunk);
			processNextChunk(model, currentChunk);
			break;
		case OBJECT_MESH:
			std::cout << "OBJECTMESH CHUNK found" << std::endl;
			processNextChunk(model, currentChunk);
			break;
		case OBJECT_VERTICES:
			std::cout << "OBJECTVERTICES CHUNK found" << std::endl;
			readVertices(model, currentChunk);
			break;
		case OBJECT_FACES:
			std::cout << "OBJECTFACES CHUNK found" << std::endl;
			readIndices(model, currentChunk);
			break;
		default:			
			int seek = currentChunk->length - currentChunk->bytesRead;
			fseek(filePointer, seek, SEEK_CUR);
			currentChunk->bytesRead += seek;
			break;
		}
		previousChunk->bytesRead = previousChunk->bytesRead + currentChunk->bytesRead;
	}
	delete currentChunk;
	currentChunk = previousChunk;
}

void Loader3DS::readChars(Chunk* previousChunk) {
	int i = 0;
	char c;
	do {
		previousChunk->bytesRead += fread(&c, 1, 1, filePointer);
		i++;
		std::cout << c;
	} while (c != '\0' && i < 20);

}

void Loader3DS::readVertices(Model* model, Chunk* previousChunk) {
	Mesh tempMesh;
	unsigned short int vertSize = 0;
	previousChunk->bytesRead += fread(&vertSize, 1, 2, filePointer);
	for (int i = 0; i < vertSize; i++)
	{
		Vertex tempVert;
		glm::vec3 tempVec;
		previousChunk->bytesRead += fread(&(tempVec.x), 1, 4, filePointer);
		previousChunk->bytesRead += fread(&(tempVec.z), 1, 4, filePointer);
		previousChunk->bytesRead += fread(&(tempVec.y), 1, 4, filePointer);
		tempVert.Position = tempVec;
		if (tempVec == glm::vec3(0.f, 0.f, 0.f)) std::cout << "0 vector!" << std::endl;
		tempMesh.vertices.push_back(tempVert);
	}
	model->meshes.push_back(tempMesh);
	std::cout << tempMesh.vertices.size() << " vertices (may be) successfuly read!" << std::endl;
}

void Loader3DS::readIndices(Model* model, Chunk* previousChunk) {
	static int times = 0;
	unsigned short int indSize = 0;
	previousChunk->bytesRead += fread(&indSize, 1, 2, filePointer);
	for (int i = 0; i < indSize; i++) {
		for (int j = 0; j < 3; j++) {
			GLuint index = 0;
			previousChunk->bytesRead += fread(&index, 1, 2, filePointer);
			model->meshes.back().indices.push_back(index);
		}
		fseek(filePointer, 2, SEEK_CUR);
		previousChunk->bytesRead += 2;
	}
	std::cout << model->meshes[times].indices.size() << " indices (may be) successfuly read!" << std::endl;
}