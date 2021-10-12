#pragma once
#include <global.hpp>
#include <voxel.hpp>
#include <PerlinNoise.h>

struct Chunk {

	TileType data[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_DEPTH];
	uint x, y;

	void (*updateCallback)(void *);
	void* obj;

	Chunk();
	Chunk(uint x, uint y);

	uint** genHeightMap();
	void genChunkMap();

	void updateChunk();
};

