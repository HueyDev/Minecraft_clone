#include <global.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

namespace ve{

    struct chunk{
        VoxelType data[CHUNK_WIDTH][CHUNK_HEIGHT][CHUNK_DEPTH];
        glm::vec2 pos;
    };

    VoxelType data[LOAD_DISTANCE*LOAD_DISTANCE];

    uint viewRange = 2;


    void loadChunk(int x, int y){
        //TODO - Check if chunk saved
        //else

        
    }

    void unloadChunk(int x, int y){

    }

    glm::vec3 worldToChunk(){

    }

    glm::vec2 worldToChunkLoc(glm::vec3 pos){
        return glm::vec2(floor(pos.x/CHUNK_WIDTH), floor(pos.z/CHUNK_DEPTH));
    }

    VoxelType *getChunk(glm::vec2 pos){
        for(int i = 0; i < LOAD_DISTANCE*LOAD_DISTANCE; i++){

        }
    }

}