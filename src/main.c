#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <time.h>

// TODO: 2026-07-27 00:59 - Nias: change colors/look with shaders
/* TODO: 2026-07-27 01:00 - Nias: add flip animation
 * For the animation we can display the frames like this:
 * 0 ....
 * 1 ....
 * 2 ....
 * where in `....` we put the falling animation right before the next tile.
 * In here we can create a better struct for the tiles and they all hold
 * their sprites so no offset calculations hell is needed.
 * When the time we get is different from the one we already have, we start the
 * animation.
 * We can animate even the middle colons but I prefrer to leave them still.
 */
// TODO: 2026-07-31 19:20 - Nias: Add AM - PM toggle
/* TODO: 2026-07-31 19:21 - Nias: Embed the assets inside the file
 * <https://stackoverflow.com/questions/4158900/embedding-resources-in-executable-using-gcc>
 */

#define TILE_W 30
#define TILE_H 65

void DrawDigit(Texture2D sheet, int digit, int x, int y, float scale) {
  // Select the tile
  Rectangle src = {digit * TILE_W, 0, TILE_W, TILE_H};

  // Scale the tile
  Rectangle dst = {x, y, TILE_W * scale, TILE_H * scale};
  
  // Draw on the screen
  DrawTexturePro(sheet, src, dst, (Vector2){0, 0}, 0, WHITE);
}

void DrawColon(Texture2D sheet, int x, int y, float scale) {
  // Select the colon tile
  Rectangle src = {2 * TILE_W, TILE_H, TILE_W, TILE_H};

  // Scale the tile
  Rectangle dst = {x, y, TILE_W * scale, TILE_H * scale};

  // Draw on the screen
  DrawTexturePro(sheet, src, dst, (Vector2){0, 0}, 0, WHITE);
}

int main(int argc, char **argv) {
  int screen_width = 800;
  int screen_height = 600;

  // Enable FullScreeen, VSync and Transparency
  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT |
                 FLAG_WINDOW_TRANSPARENT);
  InitWindow(screen_width, screen_height, "Tiles-Clock");

  // Preapre the spritesheet
  Texture2D sheet = LoadTexture("assets/Tiles.png");
  // Nearest-Neighbour Upscaling
  SetTextureFilter(sheet, TEXTURE_FILTER_POINT);
  // Shader shader = LoadShader(0, "src/blur.frag");
  // int scale_loc = GetShaderLocation(shader, "pixelScale");
  // float pixel_scale = 2.0f;
  // SetShaderValue(shader, scale_loc, &pixel_scale, SHADER_UNIFORM_FLOAT);

  Color bg = {39, 39, 39, 255};

  int spacing = 4;
  int base_width = (TILE_W * 8) + (spacing * 7);
  int base_height = TILE_H;

  while (!WindowShouldClose()) {
    // Get the time
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    // Get the proper scaling factor
    float scale_x = (float)GetScreenWidth() / base_width;
    float scale_y = (float)GetScreenHeight() / base_height;
    int scale = (int)floorf(fminf(scale_x, scale_y));
    if (scale < 1)
      scale = 1;

    // Calculate the size of each tile
    int digit_width = TILE_W * scale;
    int digit_height = TILE_H * scale;
    spacing *= scale;
    // Calculate the whole clock size
    int clock_width = (digit_width * 8) + (spacing * 7);
    int clock_height = digit_height;

    // Starting position
    int start_x = (GetScreenWidth() - clock_width) / 2;
    int start_y = (GetScreenHeight() - clock_height) / 2;

    int x = start_x; // Just a rename for clarity

    BeginDrawing();
    // BeginShaderMode(shader);
    ClearBackground(bg);

    // Hours
    DrawDigit(sheet, tm->tm_hour / 10, x, start_y, scale);
    x += digit_width + spacing;
    DrawDigit(sheet, tm->tm_hour % 10, x, start_y, scale);
    x += digit_width + spacing;

    // :
    DrawColon(sheet, x, start_y, scale);
    x += digit_width + spacing;

    // Minutes
    DrawDigit(sheet, tm->tm_min / 10, x, start_y, scale);
    x += digit_width + spacing;
    DrawDigit(sheet, tm->tm_min % 10, x, start_y, scale);
    x += digit_width + spacing;

    // :
    DrawColon(sheet, x, start_y, scale);
    x += digit_width + spacing;

    // Seconds
    DrawDigit(sheet, tm->tm_sec / 10, x, start_y, scale);
    x += digit_width + spacing;
    DrawDigit(sheet, tm->tm_sec % 10, x, start_y, scale);
    x += digit_width + spacing;

    // EndShaderMode();
    EndDrawing();
  }

  // UnloadShader(shader);
  // Cleanup
  UnloadTexture(sheet);
  CloseWindow();
  return 0;
}
