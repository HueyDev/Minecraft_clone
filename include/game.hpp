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

	RenderChunk *chunks[NUMBER_OF_CHUNKS];
	uint numberOfChunks = 1;

	Game();
	~Game();

	void render();
	void constructImages();
};