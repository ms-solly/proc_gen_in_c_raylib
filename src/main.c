#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include<stdlib.h>
#include<inttypes.h>

#define FNL_IMPL
#include "../res/FastNoiseLite/C/FastNoiseLite.h"
//#define FASTNOISELITE_H

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 600


typedef enum {
	TILE_TYPE_GRASS,
	TILE_TYPE_DIRT,
	TILE_TYPE_WATER,
} TyleType;

#define WORLD_WIDTH  SCREEN_WIDTH 
#define WORLD_HEIGHT SCREEN_HEIGHT

TyleType world_data[WORLD_HEIGHT][WORLD_WIDTH];

static void generate_world(void){
	//unsigned int seed = 90000;
	//srand(seed); // seed

	//FastNoiseLite noise;
	fnl_state noise = fnlCreateState();
	noise.noise_type = FNL_NOISE_OPENSIMPLEX2;
	noise.seed = (uint32_t)time(NULL);
	//noise.frequency = 0.03f;
	
	float scale = 3.897f;
	float min_mapped = 1.0f, max_mapped = 0.0f;
	int count_grass = 0, count_dirt = 0, count_water = 0;
 

	//float noise_val;
	for(uint32_t y = 0; y < WORLD_HEIGHT; y++){
		for(uint32_t x = 0; x < WORLD_WIDTH; x++){
			//uint random_val = rand() % 100;
			
			float raw = fnlGetNoise2D(&noise, (float)x * scale, (float)y * scale);
			float n = (raw + 1.0f) * 0.5f;
			if (n < min_mapped) min_mapped = n;
			if (n > max_mapped) max_mapped = n;
		
			if(n < 0.40f ) {
				world_data[y][x] = TILE_TYPE_GRASS;
				count_grass++;
			} else if (n < 0.60f) {				world_data[y][x] = TILE_TYPE_DIRT;
				count_dirt++;
			}else{
				world_data[y][x] = TILE_TYPE_WATER;
				count_water++;
			}
		}
	}
}

int main() {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "C raylib template");
	SetTargetFPS(60);

generate_world();
	while (!WindowShouldClose()){
		BeginDrawing();
		//ClearBackground(GREEN);
				Color tile_color;
		const uint32_t TILE_SIZE = 16;

		int offset_x = (SCREEN_WIDTH  - (WORLD_WIDTH  * TILE_SIZE)) / 2;
		int offset_y = (SCREEN_HEIGHT - (WORLD_HEIGHT * TILE_SIZE)) / 2;
		for(uint32_t y = 0; y < WORLD_HEIGHT; y++){
			for(uint32_t x = 0; x < WORLD_WIDTH; x++){

				switch(world_data[y][x]){
					case TILE_TYPE_GRASS: tile_color = GREEN;
						break;
					case TILE_TYPE_DIRT: tile_color = BROWN;
						break;
					case TILE_TYPE_WATER: tile_color = BLUE;
						break;
				}
				DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, tile_color);
			}
		}


		//DrawText("C Template", GetScreenWidth()/2 - MeasureText("C Template",100), GetScreenHeight()/2, 100, WHITE);


        EndDrawing();
    }
    

    CloseWindow();
    return 0;
}
