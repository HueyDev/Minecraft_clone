#pragma once
#include <vector>

struct TextureBuilder{

  std::vector<unsigned char*> textureData;

  TextureBuilder();

  unsigned char* loadTexture(const char* path);
  void addTexture()

}


