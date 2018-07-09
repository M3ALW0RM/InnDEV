#define TINYOBJLOADER_IMPLEMENTATION
#include "Renderable.h"
#include "GlobalFunctions.h"
#include "../ThirdParty/TinyObj/tiny_obj_loader.h"
#include <glm.hpp>
#include <random>
#include <iostream>
#include <GL/glew.h>
#include <map>
#include <string>
bool getSimilarVertexIndex(
	VertexData & packed,
	std::map<VertexData, unsigned short> & VertexToOutIndex,
	unsigned short & result
) {
	std::map<VertexData, unsigned short>::iterator it = VertexToOutIndex.find(packed);
	if (it == VertexToOutIndex.end()) {
		return false;
	}
	else {
		result = it->second;
		return true;
	}
}



Renderable::Renderable()
{
}


Renderable::~Renderable()
{
}

void Renderable::LoadOBJ(const char * filename, const char * path)
{
	std::string err;
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename, path);



	if (!err.empty()) { // `err` may contain warning message.
		std::cerr << err << std::endl;
	}

	if (!ret) {
		exit(1);
	}

	for (int i = 0; i < materials.size(); ++i)
	{
		diffuseTexID.push_back(ilutGLLoadImage(&(std::string(path) + "/" + materials[i].diffuse_texname)[0]));
	}
	shapeCount = shapes.size();

	std::map<VertexData, unsigned short> VertexToOutIndex;

	for (int i = 0; i < shapes.size(); ++i)
	{
		interleavedBufferDatas.emplace_back();
		indexBufferDatas.emplace_back();
		usedMat.push_back(shapes[i].mesh.material_ids[0]);
		for (int j = 0; j < shapes[i].mesh.indices.size(); ++j) 
		{
			//Make the complete vertex
			VertexData vertex;
			vertex.pos = { attrib.vertices[shapes[i].mesh.indices[j].vertex_index * 3],attrib.vertices[shapes[i].mesh.indices[j].vertex_index * 3 + 1], attrib.vertices[shapes[i].mesh.indices[j].vertex_index * 3 + 2] };
			vertex.colour = { attrib.colors[shapes[i].mesh.indices[j].vertex_index * 3],attrib.colors[shapes[i].mesh.indices[j].vertex_index * 3 + 1], attrib.colors[shapes[i].mesh.indices[j].vertex_index * 3 + 2] };
			if(attrib.normals.size() > 0)
				vertex.normal = { attrib.normals[shapes[i].mesh.indices[j].normal_index * 3], attrib.normals[shapes[i].mesh.indices[j].normal_index * 3 + 1] , attrib.normals[shapes[i].mesh.indices[j].normal_index * 3 + 2] };
			if (attrib.texcoords.size() > 0)
				vertex.uv = { attrib.texcoords[shapes[i].mesh.indices[j].texcoord_index * 2], attrib.texcoords[shapes[i].mesh.indices[j].texcoord_index * 2 + 1] };
			unsigned short index = 0;
			bool found = getSimilarVertexIndex(vertex, VertexToOutIndex, index);
			if (found) { // A similar vertex is already in the VBO, use it instead !
				indexBufferDatas[i].push_back(index);
			}
			else { // If not, it needs to be added in the output data.
				interleavedBufferDatas[i].push_back(vertex);
				unsigned short newindex = (unsigned short)interleavedBufferDatas[i].size() - 1;
				indexBufferDatas[i].push_back(newindex);
				VertexToOutIndex[vertex] = newindex;
			}
		}
	}
	BindData();
	Clean();
}

void Renderable::BindData()
{
	for (int i = 0; i < shapeCount; ++i)
	{
		interleavedBufferIDs.emplace_back();
		indexBufferIDs.emplace_back();
		// Generate 1 buffer, put the resulting identifier in vertexbuffer
		glGenBuffers(1, &interleavedBufferIDs[i]);
		// The following commands will talk about our 'vertexbuffer' buffer
		glBindBuffer(GL_ARRAY_BUFFER, interleavedBufferIDs[i]);
		// Give our vertices to OpenGL.
		//Data is packed (VVVCCCNNNTT)
		glBufferData(GL_ARRAY_BUFFER, interleavedBufferDatas[i].size() * sizeof(VertexData), &interleavedBufferDatas[i][0], GL_STATIC_DRAW);

		// Generate a buffer for the indices
		glGenBuffers(1, &indexBufferIDs[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferIDs[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferDatas[i].size() * sizeof(unsigned short), &indexBufferDatas[i][0], GL_STATIC_DRAW);
		indexCount.push_back(indexBufferDatas[i].size());
	}
}

void Renderable::Clean()
{
	for (int i = 0; i < shapeCount; ++i)
	{
		interleavedBufferDatas.clear();
		indexBufferDatas.clear();
	}

}
