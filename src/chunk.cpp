#include <chunk.hpp>
#include <renderChunk.hpp>

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
					this->data[j][i][z] = DIRT;
				}
				else if (i == heightMap[j][z]) {
					this->data[j][i][z] = DIRT;
				}
				else {
					//std::cout << "SET DIRT\n";
					this->data[j][i][z] = DIRT;
				}
			}
		}
	}

	delete [] heightMap;


}

uint** Chunk::genHeightMap() {

	uint** data = 0;

	data = new uint*[CHUNK_WIDTH];

	PerlinNoise pn = PerlinNoise(WORLD_SEED);

	for (int j = 0; j < CHUNK_WIDTH; j++) {
		data[j] = new uint[CHUNK_DEPTH];
		for (int i = 0; i < CHUNK_DEPTH; i++) {
			data[j][i] = (uint)(pn.noise((float)(j+this->x*CHUNK_WIDTH) / maxIntSize(), (float)(i+this->y*CHUNK_DEPTH) / maxIntSize(), 0.1f) * (MAX_HEIGHT-MIN_HEIGHT)) + MIN_HEIGHT;
			std::cout << pn.noise((float)(j + this->x * CHUNK_WIDTH) / maxIntSize(), (float)(i + this->y * CHUNK_DEPTH) / maxIntSize(), 0.1f) << std::endl;
		}
	}

	return data;

}

void Chunk::updateChunk() {
	this->updateCallback(this->obj);
}


