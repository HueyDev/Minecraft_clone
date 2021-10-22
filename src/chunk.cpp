#include <chunk.hpp>
#include <renderChunk.hpp>
#include <FastNoise.hpp>

Chunk::Chunk() {
	this->x = 0;
	this->y = 0;
}

Chunk::Chunk(uint x, uint y) {
	this->x = x;
	this->y = y;

	this->genChunkMap();
}

void Chunk::genChunkMap() {

	uint** heightMap = this->genHeightMap();

	for (int j = 0; j < CHUNK_WIDTH; j++) {
		for (int i = 0; i < CHUNK_HEIGHT; i++) {
			for (int z = 0; z < CHUNK_DEPTH; z++) {
				if (i > heightMap[j][z]) {
					//std::cout << "SET AIR\n";
					this->data[j][i][z] = AIR;
				}
				else if (i == heightMap[j][z]) {
					this->data[j][i][z] = GRASS;
				}
				else {
					//std::cout << "SET TEST_VOXEL\n";
					this->data[j][i][z] = DIRT;

					/*if (i > 6) {
						std::cout << "POS: " << j << "," << i << "," << z << "  |  Height Map: " << heightMap[j][z] << std::endl;
					}*/

				}
			}
		}
	}

	delete [] heightMap;


}

uint** Chunk::genHeightMap() {

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
			data[j][i] = (uint)(pn.GetNoise((float)(j + this->x * CHUNK_WIDTH) / MAP_SIZE, (float)(i + this->y * CHUNK_DEPTH) / MAP_SIZE) * (MAX_HEIGHT - MIN_HEIGHT) + MIN_HEIGHT);
			/*if (data[j][i] > 16) {
				std::cout << pn.GetNoise((float)(j + this->x * CHUNK_WIDTH) / MAP_SIZE, (float)(i + this->y * CHUNK_DEPTH) / MAP_SIZE) << " * (" << MAX_HEIGHT << " - " << MIN_HEIGHT << ") + " << MIN_HEIGHT << "  |  RESULT: " << data[j][i] << std::endl;
			}*/
			//std::cout << pn.GetNoise((float)(j + this->x * CHUNK_WIDTH) / MAP_SIZE, (float)(i + this->y * CHUNK_DEPTH) / MAP_SIZE) << std::endl;
			//std::cout << data[j][i] << std::endl;
		}
	}

	return data;

}

void Chunk::updateChunk() {
	this->updateCallback(this->obj);
}


