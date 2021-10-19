7#include <game.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdlib.h>

uint buildTexture(const char* filePath) {

    std::cout << "Compiling image at " << filePath << std::endl;

    //stbi_set_flip_vertically_on_load(true);

    uint tex = 0;
    int width, height, nrChannels;

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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

    std::cout << "Result: " << tex << std::endl;

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

std::pair<TileType, uint*> buildImageBox(const char* v1, const char* v2, const char* v3, const char* v4, const char* v5, const char* v6, TileType t) {
    uint* value = (uint*)malloc(6 * sizeof(uint));
    value[0] = buildTexture(v1);
    value[1] = buildTexture(v2);
    value[2] = buildTexture(v3);
    value[3] = buildTexture(v4);
    value[4] = buildTexture(v5);
    value[5] = buildTexture(v6);
    return std::pair<TileType, uint*>(t, value);
}

void Game::constructImages() {
    // ORDER: TOP, BOTTOM, RIGHT, LEFT, BACK, FRONT

    //dirt
    uint dirtTex = buildTexture("dirt.png");
    this->images.insert(buildImageBox(dirtTex, dirtTex, dirtTex, dirtTex, dirtTex, dirtTex, DIRT));

    //TEST_VOXEL
    this->images.insert(buildImageBox("test_top.png", "test_bottom.png", "test_right.png", "test_left.png", "test_back.png", "test_front.png", TEST_VOXEL));

    //GRASS
    uint grassTop = buildTexture("grass_top.png");
    uint grassSide = buildTexture("grass_side.png");
    this->images.insert(buildImageBox(grassTop, dirtTex, grassSide, grassSide, grassSide, grassSide, GRASS));

}

Game::Game() {
    std::cout << "BUILDING GAME\n";
    //buildVertex();

    this->shader = new Shader("voxel.vs","voxel.fs");
    this->camera = new Camera(glm::vec3(0.0f, 0.0f, -3.0f));

    this->shader->use();
    this->shader->setInt("top", 0);
    this->shader->setInt("bottom", 1);
    this->shader->setInt("right", 2);
    this->shader->setInt("left", 3);
    this->shader->setInt("back", 4);
    this->shader->setInt("front", 5);

    this->constructImages();

    /*Chunk* c = new Chunk(0, 0);
    Chunk* ch = new Chunk(1, 0);

    this->chunks[0] = new RenderChunk(&(this->images), 0, 0, c);
    this->chunks[1] = new RenderChunk(&(this->images), 1, 0, ch);*/

    for (int i = 0; i < WORLD_CHUNK_WIDTH; i++) {
        for (int j = 0; j < WORLD_CHUNK_HEIGHT; j++) {
            Chunk* c = new Chunk(i, j);
            this->chunks[i*WORLD_CHUNK_HEIGHT + j] = new RenderChunk(&(this->images), i, j, c);
        }
    }

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
  //std::cout << "render\n";
    this->shader->use();

    glm::mat4 transform = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.1f, 100.0f) * this->camera->GetViewMatrix();

    this->shader->setMat4("transform", transform);
    
    for (int i = 0; i < this->numberOfChunks; i++) {
        this->chunks[i]->render(this->shader);
    }
}







