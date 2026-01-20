/* the viewport is smth like
(x0,y0)
----------------------------- (x1,y0)
|                           |
|                           |
|                           |
|                           |
|                           |
|                           |
|                           |
----------------------------- (x1,y1)
(x0,y1)
*/
#include "raylib.h"

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

#define BALLS 10

typedef struct {
  float x, y;
} Coordinates;

// typedef enum { LEFT, RIGHT, TOP, BOTTOM } COLLIDED;

typedef struct {
  Coordinates coord;
  float r, vx, vy;
  Color color;
  // COLLIDED collided;
} Particle;

Particle particles[BALLS];
// 0,600 -> 0,1 => 0,1/600
void update_particle(Particle *);
void update_particles();
void draw_particle(Particle *);
void draw_particles();
// draw circle
void draw_particle(Particle *particle) {
  DrawCircle(particle->coord.x, particle->coord.y, particle->r,
             particle->color);
}

void draw_particles() {
  for (int i = 0; i < BALLS; i++) {
    draw_particle(particles + i);
  }
}

// changing particle position depending upon velocity
void update_particle(Particle *particle) {
  particle->coord.x += particle->vx;
  particle->coord.y += particle->vy;

  float x = particle->coord.x;
  float y = particle->coord.y;
  float radius = particle->r;

  if (x - radius < 0) { // left wall

    particle->coord.x = radius;
    particle->vx = -particle->vx;
  }
  if (x + radius > WINDOW_WIDTH) { // right wall

    particle->coord.x = WINDOW_WIDTH - radius;
    particle->vx = -particle->vx;
  }
  if (y + radius > WINDOW_HEIGHT) { // bottom wall

    particle->coord.y = WINDOW_HEIGHT - radius;
    particle->vy = -particle->vy;
  }
  if (y - radius < 0) { // top wall

    particle->coord.y = radius;
    particle->vy = -particle->vy;
  }
}

void update_particles() {
  for (int i = 0; i < BALLS; i++) {
    update_particle(particles + i);
  }
}

void InitParticles() {
  for (int i = 1; i < BALLS; i++) {
    particles[i] = (Particle){
        {GetRandomValue(0, WINDOW_WIDTH), GetRandomValue(0, WINDOW_HEIGHT)},
        50,
        GetRandomValue(-10, 10),
        GetRandomValue(-10, 10),
        RED};
  }
}

int main(int argc, char *argv[]) {
  // CPU consumption control
  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
             "Smashing balls using physics, I guess");
  SetTargetFPS(60);
  // Particle particle = {{300, 300}, 50, 10, 10, RED};
  InitParticles();
  while (!WindowShouldClose() && !IsKeyPressed(KEY_Q)) {
    BeginDrawing();
    DrawFPS(10, 10);
    ClearBackground(BLACK);
    draw_particles();
    update_particles();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
