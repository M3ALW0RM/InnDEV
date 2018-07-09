#pragma once
#include <GL/glew.h>
#include <vector>
#include <glm.hpp>
#include <IL/ilut.h>
struct VertexData
{
	glm::vec3 pos;
	glm::vec3 colour;
	glm::vec3 normal;
	glm::vec2 uv;
	bool operator<(const VertexData that) const {
		return memcmp((void*)this, (void*)&that, sizeof(VertexData))>0;
	};
};

class Renderable
{

public:
	int shapeCount;
	std::vector<std::vector<VertexData>> interleavedBufferDatas;
	std::vector<std::vector<unsigned short>> indexBufferDatas;
	std::vector<GLuint> diffuseTexID;
	std::vector<GLuint> interleavedBufferIDs;
	std::vector<GLuint> indexBufferIDs;
	std::vector<int> usedMat;
	std::vector<int> indexCount;
	Renderable();
	~Renderable();
	void LoadOBJ(const char* filename, const char* path);
	void BindData();
	void Clean();
};