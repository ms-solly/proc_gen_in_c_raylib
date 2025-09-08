#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include<stdlib.h>
#include<inttypes.h>

#define FNL_IMPL
#include "../res/include/FastNoiseLite/C/FastNoiseLite.h"
//#define FASTNOISELITE_H

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 600


typedef enum {
	TILE_TYPE_GRASS,
	TILE_TYPE_DIRT,
	TILE_TYPE_WATER,
} TyleType;

typedef enum{
	TILE_BASE_DIRT,
	TILE_BASE_WATER,
}TileBase;

typedef enum{
	TILE_FEATURE_NONE,
	TILE_FEATURE_GRASS
}TileFeature;

typedef struct{
	TileBase base;
	TileFeature feature;
}Tile;

typedef enum{
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
	DIRECTION_UP,
	DIRECTION_DOWN
}Direction;


typedef struct{
	Vector2 position;
	float speed;
	Texture texture;
}Player;

#define WORLD_WIDTH  SCREEN_WIDTH 
#define WORLD_HEIGHT SCREEN_HEIGHT

TyleType world_data[WORLD_HEIGHT][WORLD_WIDTH];

// Tile world_tiles[WORLD_HEIGHT][WORLD_WIDTH];

/*
* static void generate_world(void){
	//unsigned int seed = 90000;
	//srand(seed); // seed

	//FastNoiseLite noise;
	fnl_state noise = fnlCreateState();
	noise.noise_type = FNL_NOISE_OPENSIMPLEX2;
	noise.seed = (uint32_t)time(NULL);
	//noise.frequency = 0.03f;

	fnl_state feature_noise = fnlCreateState();
	feature_noise.seed = 5678;


	float scale = 8.897f;
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
*/ 
static void generate_world(void){
	//unsigned int seed = 90000;
	//srand(seed); // seed

	//FastNoiseLite noise;
	fnl_state noise = fnlCreateState();
	noise.noise_type = FNL_NOISE_OPENSIMPLEX2;
	noise.seed = (uint32_t)time(NULL);
	//noise.frequency = 0.03f;

	fnl_state feature_noise = fnlCreateState();
	feature_noise.seed = 5678;


	float scale = 8.897f;
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

static Player* create_player(Player *player, Vector2 position, float speed, Texture texture){
	player->position = position;
	player->speed = speed;
	player->texture = texture;

	return player;
}




int main() {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "C raylib template");
	SetTargetFPS(60);

	Player player;
	player.texture = LoadTexture("res/art/sheep.png");
	player.position = (Vector2){20,15};
	generate_world();
	create_player(&player, player.position, 4.0f, player.texture);
	while (!WindowShouldClose()){

		int next_x = player.position.x;
		int next_y = player.position.y;

		if (IsKeyPressed(KEY_RIGHT)) {
			next_x++;
		}
		if (IsKeyPressed(KEY_LEFT)) {
			next_x--;
		}
		if (IsKeyPressed(KEY_UP)) {
			next_y--;
		}
		if (IsKeyPressed(KEY_DOWN)) {
			next_y++;
		}

		if (next_x >= 0 && next_x < WORLD_WIDTH && next_y >= 0 && next_y < WORLD_HEIGHT) {

			// Get the type of tile at our target destination
			TyleType target_tile = world_data[next_y][next_x];
			// THIS IS THE ANSWER TO YOUR QUESTION:
			// We check if the target tile is walkable
			if (target_tile == TILE_TYPE_GRASS || target_tile == TILE_TYPE_DIRT) {
				// 3. If it's valid, update the player's actual position 
				player.position.x = next_x;
				player.position.y = next_y;
			}
        // If the tile was WATER, we do nothing! The player doesn't move.
   }
		BeginDrawing();
		//ClearBackground(GREEN);
				Color tile_color;
		const uint32_t TILE_SIZE = 16;

		int offset_x = (SCREEN_WIDTH  - (WORLD_WIDTH  * TILE_SIZE)) / 2;
		int offset_y = (SCREEN_HEIGHT - (WORLD_HEIGHT * TILE_SIZE)) / 2;
		for(unsigned long int y = 0; y < WORLD_HEIGHT; y++){
			for(unsigned long int x = 0; x < WORLD_WIDTH; x++){

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
		
				DrawRectangle(player.position.x * TILE_SIZE, player.position.y *
     TILE_SIZE, TILE_SIZE, TILE_SIZE, RED);

	Rectangle src = { 0, 0, 300, 300 }; 
	Rectangle dest = { player.position.x * TILE_SIZE, player.position.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
	DrawTexturePro(player.texture, src, dest, (Vector2){0, 0}, 0, WHITE);
    

		EndDrawing();
	}
	UnloadTexture(player.texture);
	CloseWindow();
	return 0;
}