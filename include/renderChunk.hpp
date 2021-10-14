#pragma once
#include <global.hpp>
#include <chunk.hpp>
#include <map>
#include <shader.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iterator>

enum Face {
	TOP, BOTTOM, RIGHT, LEFT, BACK, FRONT
};

struct VoxelRenderDataTemp {
	uint index = 0;
	std::vector<float> vertex;
	std::vector<uint> EBO;
};

struct RenderChunk {

	uint x, y, numOfVoxels;

	Chunk *chunk;

	std::map<TileType, VoxelRenderDataTemp*> data;
	std::map<TileType, uint*>* images;

	uint* renderId;

	void updateMesh();


	void buildFace(TileType t, Face f, glm::vec3 loc);
	void buildBuffers();

	void render(Shader *s);

	RenderChunk();
	RenderChunk(std::map<TileType, uint*>* images, uint x, uint y, Chunk *c);

	~RenderChunk();

};





