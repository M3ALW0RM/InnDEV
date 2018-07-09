#pragma once
#include <vector>
#include <map>
#include <glm.hpp>

namespace tinyobj {
	typedef float real_t;
}

struct PackedVertex {
	glm::vec3 vertex;
	glm::vec2 uv;
	glm::vec3 normal;
	bool operator<(const PackedVertex that) const {
		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
	};
};

bool getSimilarVertexIndex_fast(
	PackedVertex & packed,
	std::map<PackedVertex, unsigned short> & VertexToOutIndex,
	unsigned short & result
) {
	std::map<PackedVertex, unsigned short>::iterator it = VertexToOutIndex.find(packed);
	if (it == VertexToOutIndex.end()) {
		return false;
	}
	else {
		result = it->second;
		return true;
	}
}

void IndexVBO(
	std::vector<tinyobj::real_t>& in_vertices,
	std::vector<tinyobj::real_t>& in_uvs,
	std::vector<tinyobj::real_t>& in_normals,
	std::vector<unsigned short>& out_indices,
	std::vector<glm::vec3>& out_vertices,
	std::vector<glm::vec2>& out_uvs,
	std::vector<glm::vec3>& out_normals)
{
	std::map<PackedVertex, unsigned short> VertexToOutIndex;
	// For each input vertex
	for (unsigned int i = 0; i<in_vertices.size()/3; i++) {

		PackedVertex packed;
		packed.vertex = { in_vertices[i * 3], in_vertices[i * 3 + 1] , in_vertices[i * 3 + 2] };
		if(in_normals.size() > 0)
			packed.normal = { in_normals[i * 3], in_normals[i * 3 + 1], in_normals[i * 3 + 2] };
		if(in_uvs.size() > 0)
			packed.uv = { in_uvs[i * 2], in_uvs[i * 2 + 1] };

		// Try to find a similar vertex in out_XXXX
		unsigned short index = 0;
		bool found = getSimilarVertexIndex_fast(packed, VertexToOutIndex, index);

		if (found) { // A similar vertex is already in the VBO, use it instead !
			out_indices.push_back(index);
		}
		else { // If not, it needs to be added in the output data.
			out_vertices.push_back(packed.vertex);
			if (in_uvs.size() > 0)
			{
				out_uvs.push_back(packed.uv);
			}
			if (in_normals.size() > 0)
			{
				out_normals.push_back(packed.normal);
			}
			unsigned short newindex = (unsigned short)out_vertices.size() - 1;
			out_indices.push_back(newindex);
			VertexToOutIndex[packed] = newindex;
		}
	}
}

void InterleaveVBO(
	std::vector<glm::vec3>& in_vertices,
	std::vector<glm::vec2>& in_uvs,
	std::vector<glm::vec3>& in_normals,
	std::vector<float>& out_interleaved)
{
	for (int i = 0; i < in_vertices.size(); ++i)
	{
		out_interleaved.push_back(in_vertices[i].x);
		out_interleaved.push_back(in_vertices[i].y);
		out_interleaved.push_back(in_vertices[i].z);
	}
	for (int i = 0; i < in_uvs.size(); ++i)
	{
		out_interleaved.push_back(in_normals[i].x);
		out_interleaved.push_back(in_normals[i].y);
		out_interleaved.push_back(in_normals[i].z);
	}
	for (int i = 0; i < in_normals.size(); ++i)
	{
		out_interleaved.push_back(in_uvs[i].x);
		out_interleaved.push_back(in_uvs[i].y);
	}
}