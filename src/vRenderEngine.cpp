#include <global.hpp>
#include <iostream>
#include <vEngine.hpp>
#include <vector>
#include <map>
#include <stdlib.h>
#include <memory>
#include <player.hpp>
#include <global.hpp>
#include <shader.hpp>
#include <algorithm>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// 1      2      3      4      5      6           7         8         9
// Pos x, Pos y, Pos z, Tex x, Tex y, Texture Id, Normal x, Normal y, Normal z
#define FACE_SIZE 4*9

#define IMAGE_WIDTH 16
#define IMAGE_HEIGHT 16

//TODO Build unloading + loading system
//TODO Build render function -- DONE
//TODO add texture settings -- DONE
//TODO Rewrite shaders
//TODO Build small change chunk updater
//TODO Export function definitions to hpp

namespace vre{

  struct chunk{
    uint id;
    glm::vec2 pos;
    uint eboSize;

    chunk(uint i, glm::vec2 p, uint s){
      this->id = i;
      this->pos = p;
      this->eboSize = s;
    }
  };

  // Pos x, Pos y, Pos z, Tex x, Tex y
  // Does not include Texture id, or normal coordinates (added on in buildFace())
  float defaultFace[4*5] = {
    -VOXEL_SIZE / 2, -VOXEL_SIZE/2, 0.0f, 1.0f, 1.0f,
    VOXEL_SIZE / 2, -VOXEL_SIZE/2, 0.0f, 0.0f, 1.0f,
    VOXEL_SIZE / 2,  VOXEL_SIZE / 2, 0.0f, 0.0f, 0.0f,
    -VOXEL_SIZE / 2,  VOXEL_SIZE/2, 0.0f, 1.0f, 0.0f
  };

  chunk *chunkData[LOAD_DISTANCE*LOAD_DISTANCE];
  //                Face        Block  CHUNK
  float currentData[FACE_SIZE * 6    * CHUNK_WIDTH * CHUNK_DEPTH * CHUNK_HEIGHT];
  uint currentFaceId = 0;

  std::map<VoxelType, uint*> imageLookup;
  std::vector<const char*> imageLoc;

  Shader *shader;

  void addVao(uint id, glm::vec2 pos, uint eboSize){
    for(int i = 0; i < (LOAD_DISTANCE*LOAD_DISTANCE); i++){
      if(chunkData[i] == 0){
        chunkData[i] = new chunk(id, pos, eboSize);
        return;
      }
    }
    std::cout << "ERROR: Unable to save chunk VAO Id to chunkData buffer. No available space\n";
  }

  void sendChunkToGpu(glm::vec2 chunkPos){
    uint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, currentFaceId * sizeof(float), currentData, GL_STATIC_DRAW);
    

    uint eboData[(currentFaceId/4/9) * (6)];
    for(int i = 0; i < (currentFaceId/4/9); i++){
      uint eboLoc = i*6;
      uint vboLoc = i*4;
      eboData[eboLoc] = vboLoc;
      eboData[eboLoc] = vboLoc+1;
      eboData[eboLoc] = vboLoc+2;
      eboData[eboLoc] = vboLoc;
      eboData[eboLoc] = vboLoc+2;
      eboData[eboLoc] = vboLoc+3;
    }
    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (currentFaceId/4/9) * (6) * sizeof(uint), eboData, GL_STATIC_DRAW);

    

    addVao(VAO, chunkPos, (currentFaceId/4/9) * (6) * sizeof(uint));

    currentFaceId = 0;
  }

  VoxelType getChunkDataPoint(VoxelType *data, uint x, uint y, uint z){
    return data[(x*CHUNK_HEIGHT*CHUNK_DEPTH) + (y*CHUNK_DEPTH) + z];
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

  void addFace(float *data){
    std::memcpy(chunkData[currentFaceId], data, FACE_SIZE*sizeof(float));
    currentFaceId += FACE_SIZE;
  }

  void buildFace(VoxelType t, Face f, glm::vec3 pos){

    float data[FACE_SIZE];

    for (int i = 0; i < 4; i++) {
      int ind = i * 5;
      glm::vec4 pointData = glm::translate(glm::mat4(1.0f), pos) * getBoxFace(f) * glm::vec4(defaultFace[ind], defaultFace[ind + 1], defaultFace[ind + 2], 1.0f);
      glm::vec3 normal = getFaceNormal(f);


      uint dataPos = i * (FACE_SIZE/4);
      data[dataPos] = pointData.x;
      data[dataPos+1] = pointData.y;
      data[dataPos+2] = pointData.z;
      data[dataPos+3] = defaultFace[ind+3];
      data[dataPos+4] = defaultFace[ind+4];
      //TODO get texture id -- DONE
      data[dataPos+5] = imageLookup[t][f];
      data[dataPos+6] = normal.x;
      data[dataPos+7] = normal.y;
      data[dataPos+8] = normal.z;
    }
    addFace(data);
  }
  
  void buildChunk(glm::vec2 chunkPos){
    VoxelType *data = ve::getChunk(chunkPos);

    for(int x = 0; x < CHUNK_WIDTH; x++){
      for(int y = 0; y < CHUNK_HEIGHT; y++){
        for(int z = 0; z < CHUNK_DEPTH; z++){
          VoxelType type = getChunkDataPoint(data, x, y, z);
          glm::vec3 loc = glm::vec3(x, y, z) * VOXEL_SIZE;

          if (type == AIR) {
            continue;
          }

          //TOP
          if (y == CHUNK_HEIGHT-1 || getChunkDataPoint(data, x,y + 1,z) == AIR) {
            // std::cout << "ADDING SQUARE 2\n";
            //TOP
            buildFace(type, TOP, loc);
          }

          //BOTTOM
          if (y == 0 || getChunkDataPoint(data, x, y - 1,z) == AIR) {
            // std::cout << "ADDING SQUARE 2\n";
            //BOTTOM
            buildFace(type, BOTTOM, loc);
          }

          //RIGHT
          if (x == 0 || getChunkDataPoint(data, x - 1,y,z) == AIR) {
            // std::cout << "ADDING SQUARE 2\n";
            //RIGHT
            buildFace(type, RIGHT, loc);
          }
          
          //LEFT
          if (x == CHUNK_WIDTH-1 || getChunkDataPoint(data, x + 1,y,z) == AIR) {
            // std::cout << "ADDING SQUARE 2\n";
            //LEFT
            buildFace(type, LEFT, loc);
          }

          //BACK
          if (z == CHUNK_DEPTH-1 || getChunkDataPoint(data, x,y,z+1) == AIR) {
            // std::cout << "ADDING SQUARE 2\n";
            //BACK
            buildFace(type, BACK, loc);
          }

          //FRONT
          if (z == 0 || getChunkDataPoint(data, x,y,z - 1) == AIR) {
            // std::cout << "ADDING SQUARE 2\n";
            //FRONT
            buildFace(type, FRONT, loc);
          }

        }
      }
    }

    sendChunkToGpu(chunkPos);
  }



  int getImageIndex(const char* K)
  {
    std::vector<const char*> v = imageLoc;
    
    auto it = std::find(v.begin(), v.end(), K);

    if (it != v.end())
    {
        return it - v.begin();
    }
    else {
        return -1;
    }
  }

  uint addImage(const char * path){
    int i = getImageIndex(path);
    if(i != -1){
      return i;
    }

    imageLoc.push_back(path);
    return imageLoc.size() - 1;
  }

  void buildVoxelImage(const char *v1, const char *v2, const char *v3,const char *v4, const char *v5, const char *v6, VoxelType t){
    uint *result = (uint *)malloc(6 * sizeof(uint));
    result[0] = addImage(v1);
    result[1] = addImage(v2);
    result[2] = addImage(v3);
    result[3] = addImage(v4);
    result[4] = addImage(v5);
    result[5] = addImage(v6);
    imageLookup.insert(std::pair<VoxelType, uint*>(t, result));
  }

  void compileImages(){
    uint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_3D, tex);

    std::vector<GLubyte> emptyData(IMAGE_WIDTH * IMAGE_HEIGHT * imageLoc.size() * 4, 0);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, IMAGE_WIDTH, IMAGE_HEIGHT, imageLoc.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, &emptyData[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    for(int i = 0; i < imageLoc.size(); i++){
      int width, height, nrChannels;
      const char* filePath = imageLoc[i];
      unsigned char* data = stbi_load(("resources/" + std::string(filePath)).c_str(), &width, &height, &nrChannels, 0);
      if (data)
      {
          glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, i, IMAGE_WIDTH, IMAGE_HEIGHT, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
      }
      else
      {
          std::cout << "Failed to load texture" << std::endl;
      }
      stbi_image_free(data);
    }

    glGenerateMipmap(GL_TEXTURE_3D);
  }

  void buildVoxelImages(){
    
    const char* grassTop = "grass_top.png";
    const char* grassSide = "grass_side.png";
    const char* dirt = "dirt.png";

    //Order: TOP BOTTOM RIGHT LEFT BACK FRONT

    buildVoxelImage(dirt, dirt, dirt, dirt, dirt, dirt, DIRT);
    buildVoxelImage(grassTop, dirt, grassSide, grassSide, grassSide, grassSide, GRASS);

    compileImages();
  }

  

  void init(){
    shader = new Shader("voxel.vs","voxel.fs");
    buildVoxelImages();
  }

  void render(){
    shader->use();

    glm::mat4 transform = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.1f, 100.0f) * vep::getViewMatrix();

    buildChunk(ve::worldToChunkLoc(vep::getPosition()));

    for(chunk *c : chunkData){
      if(!c){
        continue
      }
      glm::mat4 offset = glm::translate(glm::mat4(1.0f), glm::vec3(c->pos.x * CHUNK_WIDTH, 0, c->pos.y * CHUNK_DEPTH));

      shader->setMat4("offset", offset);
      glBindVertexArray(c->id);

      glDrawElements(GL_TRIANGLES, c->eboSize, GL_UNSIGNED_INT, (void*)0);
    }

    
	  


  }

}



// IF LOOP (who said its impossible)
// while(true){
//   if(statement){
//     break;
//   }
// }
