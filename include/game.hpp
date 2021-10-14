#pragma once
#include <renderChunk.hpp>
#include <map>
#include <shader.hpp>
#include <camera.hpp>

struct Game {

	std::map<TileType, uint*> images;

	uint VAO;
	Shader* shader;
	Camera* camera;

	RenderChunk *chunks[WORLD_CHUNK_WIDTH * WORLD_CHUNK_HEIGHT];
	uint numberOfChunks = WORLD_CHUNK_WIDTH * WORLD_CHUNK_HEIGHT;

	Game();
	~Game();

	void render();
	void constructImages();
};