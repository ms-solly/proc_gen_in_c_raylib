
#include <raylib.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <inttypes.h>

#define FNL_IMPL
#include "../res/include/FastNoiseLite/C/FastNoiseLite.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 600

typedef enum {
    TILE_TYPE_GRASS,
    TILE_TYPE_DIRT,
    TILE_TYPE_WATER,
} TyleType;

typedef enum {
    TILE_BASE_DIRT,
    TILE_BASE_WATER,
} TileBase;

typedef enum {
    TILE_FEATURE_NONE,
    TILE_FEATURE_GRASS
} TileFeature;

typedef struct {
    TileBase base;
    TileFeature feature;
} Tile;

typedef enum {
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
    DIRECTION_UP,
    DIRECTION_DOWN
} Direction;

typedef struct {
    Vector2 position;
    float speed;
    Texture2D texture;

    // Animation state
    int frameWidth;
    int frameHeight;
    int currentFrame;
    int maxFrames;
    float frameTime;     // How long each frame lasts
    float frameCounter;  // Accumulator
} Player;

#define WORLD_WIDTH  SCREEN_WIDTH 
#define WORLD_HEIGHT SCREEN_HEIGHT

TyleType world_data[WORLD_HEIGHT][WORLD_WIDTH];

static void generate_world(void) {
    fnl_state noise = fnlCreateState();
    noise.noise_type = FNL_NOISE_OPENSIMPLEX2;
    noise.seed = (uint32_t)time(NULL);

    float scale = 8.897f;

    for (uint32_t y = 0; y < WORLD_HEIGHT; y++) {
        for (uint32_t x = 0; x < WORLD_WIDTH; x++) {
            float raw = fnlGetNoise2D(&noise, (float)x * scale, (float)y * scale);
            float n = (raw + 1.0f) * 0.5f;
            if (n < 0.40f) {
                world_data[y][x] = TILE_TYPE_GRASS;
            } else if (n < 0.60f) {
                world_data[y][x] = TILE_TYPE_DIRT;
            } else {
                world_data[y][x] = TILE_TYPE_WATER;
            }
        }
    }
}

static void init_player(Player *player, Vector2 position, float speed, Texture2D texture) {
    player->position = position;
    player->speed = speed;
    player->texture = texture;

    player->maxFrames = 5; // Only first 5 are valid
    player->frameWidth = texture.width / 3;  // 6 frames horizontally
    player->frameHeight = texture.height/2;
    player->currentFrame = 0;
    player->frameTime = 0.15f; // 150ms per frame
    player->frameCounter = 0.0f;
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "C raylib template");
    SetTargetFPS(60);

    Player player;
    Texture2D tex = LoadTexture("res/art/sheep.png");
    init_player(&player, (Vector2){20, 15}, 4.0f, tex);
    generate_world();

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        // Movement
        int next_x = player.position.x;
        int next_y = player.position.y;
        if (IsKeyPressed(KEY_RIGHT)) next_x++;
        if (IsKeyPressed(KEY_LEFT))  next_x--;
        if (IsKeyPressed(KEY_UP))    next_y--;
        if (IsKeyPressed(KEY_DOWN))  next_y++;

        if (next_x >= 0 && next_x < WORLD_WIDTH && next_y >= 0 && next_y < WORLD_HEIGHT) {
            TyleType target_tile = world_data[next_y][next_x];
            if (target_tile == TILE_TYPE_GRASS || target_tile == TILE_TYPE_DIRT) {
                player.position.x = next_x;
                player.position.y = next_y;
            }
        }

        // Animation update
        player.frameCounter += delta;
        if (player.frameCounter >= player.frameTime) {
            player.frameCounter = 0.0f;
            player.currentFrame++;
            if (player.currentFrame >= player.maxFrames) {
                player.currentFrame = 0; // loop
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        // Draw tiles
        const uint32_t TILE_SIZE = 16;
        for (unsigned long int y = 0; y < WORLD_HEIGHT; y++) {
            for (unsigned long int x = 0; x < WORLD_WIDTH; x++) {
                Color tile_color;
                switch (world_data[y][x]) {
                    case TILE_TYPE_GRASS: tile_color = GREEN; break;
                    case TILE_TYPE_DIRT:  tile_color = BROWN; break;
                    case TILE_TYPE_WATER: tile_color = BLUE; break;
                }
                DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, tile_color);
            }
        }

        // Draw player (animated sprite)
        Rectangle src = { 
            player.frameWidth * player.currentFrame, 
            0, 
            player.frameWidth, 
            player.frameHeight 
        };
        Rectangle dest = { 
            player.position.x * TILE_SIZE, 
            player.position.y * TILE_SIZE, 
            TILE_SIZE*4, 
            TILE_SIZE*4 
        };
        DrawTexturePro(player.texture, src, dest, (Vector2){0, 0}, 0, WHITE);

        EndDrawing();
    }

    UnloadTexture(player.texture);
    CloseWindow();
    return 0;
}
