#pragma once
#include <math.h>

#define HEIGHT_MAP_FREQUENCY 10000.0f

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

//#define uint unsigned int
typedef unsigned int uint;

#define MIN_HEIGHT 100
#define MAX_HEIGHT 132

#define WORLD_SEED 250

#define CHUNK_WIDTH 16
#define CHUNK_DEPTH 16
#define CHUNK_HEIGHT 255

//#define NUMBER_OF_CHUNKS 2
#define WORLD_CHUNK_WIDTH 5
#define WORLD_CHUNK_HEIGHT 5

#define LOAD_DISTANCE 7
#define VIEW_DISTANCE 5

#define MAP_SIZE 200000.0f

#define VOXEL_SIZE 1.0f

#define CHUNK_SPACING 1

enum VoxelType {

	TEST_VOXEL, AIR, DIRT, GRASS, STONE, COBBLESTONE, WATER

};


//NUMBER DEPENDENT DO NOT CHANGE FIRST 6
enum Face {
	TOP, BOTTOM, RIGHT, LEFT, BACK, FRONT
};