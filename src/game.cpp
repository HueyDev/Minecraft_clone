#include <game.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdlib.h>

float vertex[CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT * 24 * 5];

uint index = 0;

void addVertexValue(float v) {
    //std::cout << "TEST\n";
   /* if (index > 15728640) {
        std::cout << index << std::endl;
    }*/
    vertex[index] = v;
    index++;
}

void addVertex(float v1, float v2, float v3, float v4, float v5) {
    addVertexValue(v1);
    addVertexValue(v2);
    addVertexValue(v3);
    addVertexValue(v4);
    addVertexValue(v5);
}

void buildVertex() {
    index = 0;

    for (int i = 0; i < CHUNK_WIDTH; i++) {
        for (int j = 0; j < CHUNK_HEIGHT; j++) {
            for (int k = 0; k < CHUNK_DEPTH; k++) {

                //TOP
                addVertex(0.0f + i * 0.5f, 0.5f + j * 0.5f, 0.0f + k * 0.5f, 0.0f, 0.0f);
                addVertex(0.5f + i * 0.5f, 0.5f + j * 0.5f, 0.0f + k * 0.5f, 1.0f, 0.0f);
                addVertex(0.5f + i * 0.5f, 0.5f + j * 0.5f, 0.5f + k * 0.5f, 1.0f, 1.0f);
                addVertex(0.0f + i * 0.5f, 0.5f + j * 0.5f, 0.5f + k * 0.5f, 0.0f, 1.0f);

                //BOTTOM
                addVertex(0.0f + i * 0.5f, 0.0f + j * 0.5f, 0.0f + k * 0.5f, 0.0f, 0.0f);
                addVertex(0.5f + i * 0.5f, 0.0f + j * 0.5f, 0.0f + k * 0.5f, 1.0f, 0.0f);
                addVertex(0.5f + i * 0.5f, 0.0f + j * 0.5f, 0.5f + k * 0.5f, 1.0f, 1.0f);
                addVertex(0.0f + i * 0.5f, 0.0f + j * 0.5f, 0.5f + k * 0.5f, 0.0f, 1.0f);

                //RIGHT
                addVertex(0.5f + i * 0.5f, 0.0f + j * 0.5f, 0.0f + k * 0.5f, 0.0f, 0.0f);
                addVertex(0.5f + i * 0.5f, 0.0f + j * 0.5f, 0.5f + k * 0.5f, 1.0f, 0.0f);
                addVertex(0.5f + i * 0.5f, 0.5f + j * 0.5f, 0.5f + k * 0.5f, 1.0f, 1.0f);
                addVertex(0.5f + i * 0.5f, 0.5f + j * 0.5f, 0.0f + k * 0.5f, 0.0f, 1.0f);

                //LEFT
                addVertex(0.0f + i * 0.5f, 0.0f + j * 0.5f, 0.5f + k * 0.5f, 0.0f, 0.0f);
                addVertex(0.0f + i * 0.5f, 0.0f + j * 0.5f, 0.0f + k * 0.5f, 1.0f, 0.0f);
                addVertex(0.0f + i * 0.5f, 0.5f + j * 0.5f, 0.0f + k * 0.5f, 1.0f, 1.0f);
                addVertex(0.0f + i * 0.5f, 0.5f + j * 0.5f, 0.5f + k * 0.5f, 0.0f, 1.0f);

                //BACK
                addVertex(0.5f + i * 0.5f, 0.0f + j * 0.5f, 0.5f + k * 0.5f, 0.0f, 0.0f);
                addVertex(0.0f + i * 0.5f, 0.0f + j * 0.5f, 0.5f + k * 0.5f, 1.0f, 0.0f);
                addVertex(0.0f + i * 0.5f, 0.5f + j * 0.5f, 0.5f + k * 0.5f, 1.0f, 1.0f);
                addVertex(0.5f + i * 0.5f, 0.5f + j * 0.5f, 0.5f + k * 0.5f, 0.0f, 1.0f);

                //FRONT
                addVertex(0.0f + i * 0.5f, 0.0f + j * 0.5f, 0.0f + k * 0.5f, 0.0f, 0.0f);
                addVertex(0.5f + i * 0.5f, 0.0f + j * 0.5f, 0.0f + k * 0.5f, 1.0f, 0.0f);
                addVertex(0.5f + i * 0.5f, 0.5f + j * 0.5f, 0.0f + k * 0.5f, 1.0f, 1.0f);
                addVertex(0.0f + i * 0.5f, 0.5f + j * 0.5f, 0.0f + k * 0.5f, 0.0f, 1.0f);
            }
        }
    }

}

uint buildTexture(const char* filePath) {

    uint tex = 0;
    int width, height, nrChannels;

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    unsigned char* data = stbi_load(("resources/" + std::string(filePath)).c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return tex;
}

std::pair<TileType, uint*> buildImageBox(uint v1, uint v2, uint v3, uint v4, uint v5, uint v6, TileType t) {
    uint* value = (uint*)malloc(6 * sizeof(uint));
    value[0] = v1;
    value[1] = v2;
    value[2] = v3;
    value[3] = v4;
    value[4] = v5;
    value[5] = v6;
    return std::pair<TileType, uint*>(t, value);
}

void Game::constructImages() {
    // ORDER: TOP, BOTTOM, RIGHT, LEFT, BACK, FRONT
    uint dirtTex = buildTexture("dirt.png");
    this->images.insert(buildImageBox(dirtTex, dirtTex, dirtTex, dirtTex, dirtTex, dirtTex, DIRT));
}

Game::Game() {

    buildVertex();

    this->shader = new Shader("voxel.vs","voxel.fs");
    this->camera = new Camera(glm::vec3(0.0f, 0.0f, -3.0f));

    glGenVertexArrays(1, &(this->VAO));
    glBindVertexArray(this->VAO);


    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, index, vertex, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3 * sizeof(float)));

    this->constructImages();

    Chunk* c = new Chunk(0, 0);

    this->chunks[0] = new RenderChunk(0, 0, &(this->images), c);

    std::cout << "FINISHED chunk gen\n";
}

Game::~Game() {
    delete(shader);
    delete(camera);

    for (auto const& x : this->images)
    {
        free(x.second);
    }

    
}

void Game::render() {
    //std::cout << "GAME RENDERING\n";
    glBindVertexArray(this->VAO);
    this->shader->use();

    glm::mat4 transform = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.1f, 100.0f) * this->camera->GetViewMatrix();
    
    this->shader->setMat4("transform", transform);
    
    for (int i = 0; i < this->numberOfChunks; i++) {
        this->chunks[i]->render(this->shader);
    }
}







