#include "raylib.h"

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

#define BALLS 10


int main(int argc, char *argv[]) {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Smashing balls using physics, I guess");
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Smashing balls using physics, I guess", WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2, 20, WHITE);
    EndDrawing();
  }
  CloseWindow();
}
