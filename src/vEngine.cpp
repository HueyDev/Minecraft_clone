#include <global.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <iostream>
#include <FastNoise.hpp>

//TODO check for memory leak at generateChunk with heightmap **

namespace ve {

	struct chunk {
		VoxelType data[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_DEPTH] = {TEST_VOXEL};
		glm::vec2 pos;
	};

	chunk data[LOAD_DISTANCE * LOAD_DISTANCE];

	uint viewRange = 2;

	uint** genHeightMap(glm::vec2 pos) {

		uint** data = 0;

		data = new uint * [CHUNK_WIDTH];

		//PerlinNoise pn = PerlinNoise(WORLD_SEED);
		FastNoiseLite pn;
		pn.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		//pn.SetFractalOctaves(20);
		pn.SetFrequency(HEIGHT_MAP_FREQUENCY);

		for (int j = 0; j < CHUNK_WIDTH; j++) {
			data[j] = new uint[CHUNK_DEPTH];
			for (int i = 0; i < CHUNK_DEPTH; i++) {
				data[j][i] = (uint)(pn.GetNoise((float)(MAP_SIZE/2 + j + pos.x * CHUNK_WIDTH) / MAP_SIZE, (float)(MAP_SIZE/2 + i + pos.y * CHUNK_DEPTH) / MAP_SIZE) * (MAX_HEIGHT - MIN_HEIGHT) + MIN_HEIGHT);
				/*if (data[j][i] > 16) {
					std::cout << pn.GetNoise((float)(j + this->x * CHUNK_WIDTH) / MAP_SIZE, (float)(i + this->y * CHUNK_DEPTH) / MAP_SIZE) << " * (" << MAX_HEIGHT << " - " << MIN_HEIGHT << ") + " << MIN_HEIGHT << "  |  RESULT: " << data[j][i] << std::endl;
				}*/
				//std::cout << pn.GetNoise((float)(j + this->x * CHUNK_WIDTH) / MAP_SIZE, (float)(i + this->y * CHUNK_DEPTH) / MAP_SIZE) << std::endl;
				//std::cout << data[j][i] << std::endl;
			}
		}

		return data;

	}

	chunk* findEmptyChunk() {
		for (int i = 0; i < LOAD_DISTANCE * LOAD_DISTANCE; i++) {
			if (data[i].data[0][0][0] == TEST_VOXEL) {
				return &(data[i]);
			}
		}
		return nullptr;
	}

	chunk *generateChunk(glm::vec2 pos) {
		std::cout << "Generating chunk at " << pos.x << "," << pos.y << "\n";

		chunk* c = findEmptyChunk();

		uint** heightMap = genHeightMap(pos);

		for (int j = 0; j < CHUNK_WIDTH; j++) {
			for (int i = 0; i < CHUNK_HEIGHT; i++) {
				for (int z = 0; z < CHUNK_DEPTH; z++) {
					if (i > heightMap[j][z]) {
						//std::cout << "SET AIR\n";
						c->data[j][i][z] = AIR;
					}
					else if (i == heightMap[j][z]) {
						c->data[j][i][z] = GRASS;
					}
					else {
						//std::cout << "SET TEST_VOXEL\n";
						c->data[j][i][z] = DIRT;

						/*if (i > 6) {
							std::cout << "POS: " << j << "," << i << "," << z << "  |  Height Map: " << heightMap[j][z] << std::endl;
						}*/

					}
				}
			}
		}

		delete[] heightMap;
		return c;
	}

	chunk* loadChunk(glm::vec2 pos) {
		//TODO - Check if chunk saved
		//else

		return generateChunk(pos);

	}

	void unloadChunk(int x, int y) {

	}

	glm::vec3 worldToChunk(glm::vec3 pos) {
		return glm::vec3(((int)pos.x % CHUNK_WIDTH), pos.y, (int)pos.z % CHUNK_DEPTH);
	}

	glm::vec2 worldToChunkLoc(glm::vec3 pos) {
		return glm::vec2(floor(pos.x / CHUNK_WIDTH), floor(pos.z / CHUNK_DEPTH));
	}

	VoxelType* getChunk(glm::vec2 pos) {
		for (int i = 0; i < LOAD_DISTANCE * LOAD_DISTANCE; i++) {
			if (data[i].data[0][0][0] == TEST_VOXEL) continue;
			if (data[i].pos == pos) {
				return &(data[i].data[0][0][0]);
			}
		}
		return &(loadChunk(pos)->data[0][0][0]);
	}

}