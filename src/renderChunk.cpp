#include <renderChunk.hpp>
#include <glad/glad.h>


void callback(void *t) {
	static_cast<RenderChunk*>(t)->updateMesh();
}

unsigned int maxIntSize() {
	return pow(2, sizeof(int) * 8);
}

RenderChunk::RenderChunk(uint x, uint y, std::map<TileType, uint*> *origImages, Chunk *c) {

	this->x = x;
	this->y = y;
	this->index = 0;
	this->imageIndex = 0;
	this->origImages = origImages;
	glGenBuffers(1, &(this->EBO));
	this->chunk = c;

	this->chunk->updateCallback = callback;
	this->chunk->obj = (void*)this;

	this->updateMesh();

}

void RenderChunk::addValue(uint val) {
	this->data[this->index] = val;
	this->index++;
}

void RenderChunk::addTriangle(uint x, uint y, uint z) {
	this->addValue(x);
	this->addValue(y);
	this->addValue(z);
}

void RenderChunk::addSquare(uint v1, uint v2, uint v3, uint v4, uint image) {
	
	this->addTriangle(v1, v2, v3);
	this->addTriangle(v1, v3, v4);
	this->images[this->imageIndex] = image;
	this->imageIndex++;
}

void RenderChunk::updateMesh() {
	this->index = 0;
	this->imageIndex = 0;
	
	for (int i = 0; i < CHUNK_WIDTH; i++) {
		for (int j = 0; j < CHUNK_HEIGHT; j++) {
			for (int k = 0; k < CHUNK_DEPTH; k++) {
				//std::cout << i << "," << j << "," << k << std::endl;
				uint meshLoc = i * (j * CHUNK_HEIGHT) + k;
				TileType type = this->chunk->data[i][j][k];

				if (type == AIR) {
					continue;
				}

				if (j == CHUNK_HEIGHT-1 || this->chunk->data[i][j + 1][k] == AIR) {
					// std::cout << "ADDING SQUARE 2\n";
					//TOP
					this->addSquare(meshLoc, meshLoc + 2, meshLoc + 3, meshLoc + 4, this->origImages->at(type)[0]);
				}

				meshLoc += 4;

				if (j == 0 || this->chunk->data[i][j - 1][k] == AIR) {
					// std::cout << "ADDING SQUARE 2\n";
					//BOTTOM
					this->addSquare(meshLoc, meshLoc + 2, meshLoc + 3, meshLoc + 4, this->origImages->at(type)[1]);
				}

				meshLoc += 4;

				if (i == CHUNK_WIDTH-1 || this->chunk->data[i + 1][j][k] == AIR) {
					// std::cout << "ADDING SQUARE 2\n";
					//RIGHT
					this->addSquare(meshLoc, meshLoc + 2, meshLoc + 3, meshLoc + 4, this->origImages->at(type)[2]);
				}

				meshLoc += 4;
				
				if (i == 0 || this->chunk->data[i - 1][j][k] == AIR) {
					// std::cout << "ADDING SQUARE 2\n";
					//LEFT
					this->addSquare(meshLoc, meshLoc + 2, meshLoc + 3, meshLoc + 4, this->origImages->at(type)[3]);
				}

				meshLoc += 4;

				if (k == CHUNK_DEPTH-1 || this->chunk->data[i][j][k+1] == AIR) {
					// std::cout << "ADDING SQUARE 2\n";
					//BACK
					this->addSquare(meshLoc, meshLoc + 2, meshLoc + 3, meshLoc + 4, this->origImages->at(type)[4]);
				}

				meshLoc += 4;

				if (k == 0 || this->chunk->data[i][j][k - 1] == AIR) {
					// std::cout << "ADDING SQUARE 2\n";
					//FRONT
					this->addSquare(meshLoc, meshLoc + 2, meshLoc + 3, meshLoc + 4, this->origImages->at(type)[5]);
				}
			}
		}
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->index, this->data, GL_DYNAMIC_DRAW);

}

void RenderChunk::render(Shader *s) {

	//std::cout << "RENDERING\n";

	glm::mat4 offset = glm::mat4(1.0f);
	offset = glm::translate(offset, glm::vec3(this->x, this->y, 0));

	s->setMat4("offset", offset);

	for (int i = 0; i < this->imageIndex; i++) {
		std::cout << i << std::endl;
		//std::cout << "DRAWING FACE\n";
		glBindTexture(GL_TEXTURE_2D, this->images[i]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6*i));
	}
	
}

RenderChunk::RenderChunk(){
	this->index = 0;
	this->imageIndex = 0;
	this->EBO = 0;

	this->origImages = nullptr;
	
}

