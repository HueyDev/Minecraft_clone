#include <renderChunk.hpp>
#include <glad/glad.h>

// Does not include face id (added on in buildFace())
float defaultFace[4 * 5] = {
	-VOXEL_SIZE / 2, -VOXEL_SIZE/2, 0.0f, 1.0f, 1.0f,
	 VOXEL_SIZE / 2, -VOXEL_SIZE/2, 0.0f, 0.0f, 1.0f,
	 VOXEL_SIZE / 2,  VOXEL_SIZE / 2, 0.0f, 0.0f, 0.0f,
	-VOXEL_SIZE / 2,  VOXEL_SIZE/2, 0.0f, 1.0f, 0.0f,

};

void callback(void *t) {
	static_cast<RenderChunk*>(t)->updateMesh();
}

unsigned int maxIntSize() {
	return pow(2, sizeof(int) * 8);
}

RenderChunk::RenderChunk(std::map<TileType, uint*>* images, uint x, uint y, Chunk *c) {

	this->x = x;
	this->y = y;
	this->chunk = c;
	this->images = images;

	this->chunk->updateCallback = callback;
	this->chunk->obj = (void*)this;

	this->updateMesh();

}

glm::mat4 getBoxFace(Face f) {
	glm::mat4 val = glm::mat4(1.0f);

	glm::mat4 def = glm::translate(val, glm::vec3(0.0f, 0.0f, -VOXEL_SIZE / 2));

	switch (f) {
	case TOP:
		return glm::rotate(val, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * def;
	case BOTTOM:
		return glm::rotate(val, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * def;
	case RIGHT:
		return glm::rotate(val, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * def;
	case LEFT:
		return glm::rotate(val, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * def;
	case BACK:
		return glm::rotate(val, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * def;
	case FRONT:
		return def;
	}
}

glm::vec3 getFaceNormal(Face f) {
	switch (f) {
	case TOP:
		return glm::vec3(0.0f, 1.0f, 0.0f);
	case BOTTOM:
		return glm::vec3(0.0f, -1.0f, 0.0f);
	case RIGHT:
		return glm::vec3(1.0f, 0.0f, 0.0f);
	case LEFT:
		return glm::vec3(-1.0f, 0.0f, 0.0f);
	case BACK:
		return glm::vec3(0.0f, 0.0f, 1.0f);
	case FRONT:
		return glm::vec3(0.0f, 0.0f, -1.0f);
	}
}

void RenderChunk::buildFace(TileType t, Face f, glm::vec3 loc) {
	if (this->data.find(t) == this->data.end()) {
		this->data.insert(std::pair<TileType, VoxelRenderDataTemp*>(t, new VoxelRenderDataTemp()));
	}
	VoxelRenderDataTemp* data = this->data.at(t);

	for (int i = 0; i < 4; i++) {
		int ind = i * 5;
		glm::vec4 pointData = glm::translate(glm::mat4(1.0f), loc) * getBoxFace(f) * glm::vec4(defaultFace[ind], defaultFace[ind + 1], defaultFace[ind + 2], 1.0f);
		data->vertex.push_back(pointData.x);
		data->vertex.push_back(pointData.y);
		data->vertex.push_back(pointData.z);
		data->vertex.push_back(defaultFace[ind+3]);
		data->vertex.push_back(defaultFace[ind+4]);
		data->vertex.push_back(f);

		glm::vec3 normal = getFaceNormal(f);
		data->vertex.push_back(normal.x);
		data->vertex.push_back(normal.y);
		data->vertex.push_back(normal.z);
		

		/*if (f != 0) {
			std::cout << (float)f << std::endl;
		}*/
	}

	data->EBO.push_back(data->index);
	data->EBO.push_back(data->index+1);
	data->EBO.push_back(data->index+2);
	data->EBO.push_back(data->index);
	data->EBO.push_back(data->index+2);
	data->EBO.push_back(data->index+3);

	data->index += 4;
}

void RenderChunk::buildBuffers() {
	this->numOfVoxels = 0;
	this->renderId = (uint*)malloc(this->data.size() * (1 * sizeof(uint) + 6 * sizeof(uint) + 1 * sizeof(uint)));

	uint index = 0;

	for (std::pair<TileType, VoxelRenderDataTemp*> d : this->data) {
		uint VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		uint VBO;
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		float* vertexData = (float*)malloc(d.second->vertex.size() * sizeof(float));
		std::copy(d.second->vertex.begin(), d.second->vertex.end(), vertexData);

		glBufferData(GL_ARRAY_BUFFER, d.second->vertex.size() * sizeof(float), vertexData, GL_DYNAMIC_DRAW);

		free(vertexData);


		uint EBO;
		glGenBuffers(1, &EBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


		uint* eboData = (uint*)malloc(d.second->EBO.size() * sizeof(uint));
		std::copy(d.second->EBO.begin(), d.second->EBO.end(), eboData);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, d.second->EBO.size() * sizeof(uint), eboData, GL_DYNAMIC_DRAW);
		//std::cout << d.second->EBO.size() << " SIZE\n";

		free(eboData);

		this->renderId[index] = VAO;

		uint* images = (uint*)malloc(9 * sizeof(uint));
		memcpy(images, this->images->at(d.first), 9 * sizeof(uint));

		std::cout << "Building buffer for type " << d.first << std::endl;

		this->renderId[index + 1] = images[0];
		this->renderId[index + 2] = images[1];
		this->renderId[index + 3] = images[2];
		this->renderId[index + 4] = images[3];
		this->renderId[index + 5] = images[4];
		this->renderId[index + 6] = images[5];

		this->renderId[index + 7] = d.second->EBO.size() * sizeof(uint);

		free(images);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		uint size = 9 * sizeof(float);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, size, (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, size, (void*)(3 * sizeof(float)));
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, size, (void*)(5 * sizeof(float)));
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, size, (void*)(6 * sizeof(float)));


		index += 8;

		this->numOfVoxels++;
	}


	for (std::pair<TileType, VoxelRenderDataTemp*> p : this->data) {
		delete p.second;
	}
	this->data.clear();
}

void RenderChunk::updateMesh() {
	for (int i = 0; i < CHUNK_WIDTH; i++) {
		for (int j = 0; j < CHUNK_HEIGHT; j++) {
			for (int k = 0; k < CHUNK_DEPTH; k++) {
				//std::cout << i << "," << j << "," << k << std::endl;
				glm::vec3 loc = glm::vec3(i, j, k) * VOXEL_SIZE;
				TileType type = this->chunk->data[(int)i][(int)j][(int)k];

				if (type == AIR) {
					continue;
				}

				//TOP
				if (j == CHUNK_HEIGHT-1 || this->chunk->data[i][j + 1][k] == AIR) {
					// std::cout << "ADDING SQUARE 2\n";
					//TOP
					this->buildFace(type, TOP, loc);
				}

				//BOTTOM
				if (j == 0 || this->chunk->data[i][j - 1][k] == AIR) {
					// std::cout << "ADDING SQUARE 2\n";
					//BOTTOM
					this->buildFace(type, BOTTOM, loc);
				}

				//RIGHT
				if (i == 0 || this->chunk->data[i - 1][j][k] == AIR) {
					// std::cout << "ADDING SQUARE 2\n";
					//RIGHT
					this->buildFace(type, RIGHT, loc);
				}
				
				//LEFT
				if (i == CHUNK_WIDTH-1 || this->chunk->data[i + 1][j][k] == AIR) {
					// std::cout << "ADDING SQUARE 2\n";
					//LEFT
					this->buildFace(type, LEFT, loc);
				}



				//BACK
				if (k == CHUNK_DEPTH-1 || this->chunk->data[i][j][k+1] == AIR) {
					// std::cout << "ADDING SQUARE 2\n";
					//BACK
					this->buildFace(type, BACK, loc);
				}



				//FRONT
				if (k == 0 || this->chunk->data[i][j][k - 1] == AIR) {
					// std::cout << "ADDING SQUARE 2\n";
					//FRONT
					this->buildFace(type, FRONT, loc);
				}
			}
		}
	}

	this->buildBuffers();

}

void RenderChunk::render(Shader *s) {

	//std::cout << "RENDERING\n";

	glm::mat4 offset = glm::mat4(1.0f);
	offset = glm::translate(offset, glm::vec3(this->x * CHUNK_WIDTH/2, 0, this->y * CHUNK_DEPTH/2));

	s->setMat4("offset", offset);

	for (int i = 0; i < this->numOfVoxels; i++) {

		int l = i * 8;
		glBindVertexArray(this->renderId[l]);

		for (int j = 1; j < 7; j++) {
			glActiveTexture(GL_TEXTURE0 + (j-1));

			glBindTexture(GL_TEXTURE_2D, renderId[l + j]);
		}

		//std::cout << i << std::endl;

		//std::cout << this->renderId[l + 7]/sizeof(uint) << std::endl;
		
		glDrawElements(GL_TRIANGLES, this->renderId[l+7], GL_UNSIGNED_INT, (void*)0);
	}
	
}

RenderChunk::RenderChunk(){
	this->x = 0;
	this->y = 0;
}

RenderChunk::~RenderChunk() {
	for (std::pair<TileType, VoxelRenderDataTemp*> p : this->data) {
		delete p.second;
	}
	free(this->renderId);
}

