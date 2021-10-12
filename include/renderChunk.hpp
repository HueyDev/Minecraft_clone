#pragma once
#include <global.hpp>
#include <chunk.hpp>
#include <map>
#include <shader.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


struct RenderChunk {

	uint x, y;

	Chunk *chunk;

	uint index;
	uint imageIndex;
	uint EBO;

	std::map<TileType, uint*>* origImages;

	uint data[CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT * 6 * 2 * 3];
	uint images[CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT * 6];

	void updateMesh();
	void addValue(uint val);
	void addTriangle(uint x, uint y, uint z);
	void addSquare(uint v1, uint v2, uint v3, uint v4, uint image);

	

	void render(Shader *s);

	RenderChunk();
	RenderChunk(uint x, uint y, std::map<TileType, uint*>* origImages, Chunk *c);

};





