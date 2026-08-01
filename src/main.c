#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <time.h>

// TODO: 2026-07-31 19:20 - Nias: Add AM - PM toggle
/* TODO: 2026-07-31 19:21 - Nias: Embed the assets inside the file
 * <https://stackoverflow.com/questions/4158900/embedding-resources-in-executable-using-gcc>
 */

#define TILE_W 30
#define TILE_H 65
#define TILE_FRAMES 6
#define SPEED 2

typedef struct {
  int current_digit;
  int target_digit;
  int progress;
  bool is_animating;
} DigitSlot;

void DrawDigit(Texture2D sheet, int digit, int frame, int x, int y,
               float scale) {
  Rectangle src = {frame * TILE_W, digit * TILE_H, TILE_W, TILE_H};
  Rectangle dst = {x, y, TILE_W * scale, TILE_H * scale};
  DrawTexturePro(sheet, src, dst, (Vector2){0, 0}, 0, WHITE);
}

void DrawAnimatedDigit(Texture2D sheet, DigitSlot *digit, int value, int x,
                       int start_y, float scale) {
  float progress_speed = digit->progress / SPEED;
  digit->target_digit = value;
  if (digit->current_digit != digit->target_digit) {
    if (!digit->is_animating)
      digit->is_animating = true;

    DrawDigit(sheet, digit->target_digit, 0, x, start_y, scale);
    DrawDigit(sheet, digit->current_digit, progress_speed, x, start_y, scale);

    if (progress_speed < TILE_FRAMES) {
      digit->progress++;
    } else {
      digit->progress = 0;
      digit->is_animating = false;
      digit->current_digit = digit->target_digit;
    }
  } else {
    DrawDigit(sheet, value, 0, x, start_y, scale);
  }
}

int main(int argc, char **argv) {
  int screen_width = 523;
  int screen_height = 165;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_TRANSPARENT);
  InitWindow(screen_width, screen_height, "Tiles-Clock");

  Texture2D sheet = LoadTexture("assets/Tiles.png");
  if (!IsTextureValid(sheet)) {
    fprintf(stderr, "ERROR: Error loading texture");
    return 1;
  }
  SetTextureFilter(sheet, TEXTURE_FILTER_POINT);
  SetTargetFPS(60);

  // Shader shader = LoadShader(0, "src/blur.frag");
  // int scale_loc = GetShaderLocation(shader, "pixelScale");
  // float pixel_scale = 2.0f;
  // SetShaderValue(shader, scale_loc, &pixel_scale, SHADER_UNIFORM_FLOAT);

  Color bg = {39, 39, 39, 255};

  int spacing = 0;
  int base_width = (TILE_W * 8) + (spacing * 7);
  int base_height = TILE_H;

  DigitSlot digits[6] = {0};

  while (!WindowShouldClose()) {

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    int values[6] = {tm->tm_hour / 10, tm->tm_hour % 10, tm->tm_min / 10,
                     tm->tm_min % 10,  tm->tm_sec / 10,  tm->tm_sec % 10};

    float scale_x = (float)GetScreenWidth() / base_width;
    float scale_y = (float)GetScreenHeight() / base_height;
    int scale = (int)floorf(fminf(scale_x, scale_y));
    if (scale < 1)
      scale = 1;

    int digit_width = TILE_W * scale;
    int digit_height = TILE_H * scale;
    int scaled_spacing = spacing * scale;

    int clock_width = (digit_width * 8) + (scaled_spacing * 7);
    int start_x = (GetScreenWidth() - clock_width) / 2;
    int start_y = (GetScreenHeight() - digit_height) / 2;

    int x = start_x;

    BeginDrawing();
    // BeginShaderMode(shader);
    ClearBackground(bg);

    // Hours
    DrawAnimatedDigit(sheet, &digits[0], values[0], x, start_y, scale);
    x += digit_width + scaled_spacing;
    DrawAnimatedDigit(sheet, &digits[1], values[1], x, start_y, scale);
    x += digit_width + scaled_spacing;

    DrawDigit(sheet, 12, 0, x, start_y, scale);
    x += digit_width + scaled_spacing;

    // Minutes
    DrawAnimatedDigit(sheet, &digits[2], values[2], x, start_y, scale);
    x += digit_width + scaled_spacing;
    DrawAnimatedDigit(sheet, &digits[3], values[3], x, start_y, scale);
    x += digit_width + scaled_spacing;

    DrawDigit(sheet, 12, 0, x, start_y, scale);
    x += digit_width + scaled_spacing;

    // Seconds
    DrawAnimatedDigit(sheet, &digits[4], values[4], x, start_y, scale);
    x += digit_width + scaled_spacing;
    DrawAnimatedDigit(sheet, &digits[5], values[5], x, start_y, scale);

    // EndShaderMode();
    EndDrawing();
  }

  // UnloadShader(shader);
  UnloadTexture(sheet);
  CloseWindow();
  return 0;
}
