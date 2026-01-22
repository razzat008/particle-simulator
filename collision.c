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
#include <stdbool.h>
#include <time.h>

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

#define BALLS 100

typedef struct {
  float x;
  float y;
} Coordinates;

// typedef enum { LEFT, RIGHT, TOP, BOTTOM } COLLIDED;

typedef struct {
  Coordinates coord;
  float r, vx, vy;
  Color color;
  // COLLIDED collided;
} Particle;

Particle particles[BALLS];

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

void init_particles() {
  SetRandomSeed(time(NULL));
  for (int i = 1; i < BALLS; i++) {
    particles[i] = (Particle){
        {GetRandomValue(0, WINDOW_WIDTH), GetRandomValue(0, WINDOW_HEIGHT)},
        GetRandomValue(5, 15),
        GetRandomValue(-10, 10),
        GetRandomValue(-10, 10),
        RED};
  }
}

void collide_all_particles() {
  Particle p1, p2;
  for (int i = 0; i < BALLS; i++) {
    for (int j = 0; j < BALLS; j++) {
      if (i == j) {
        return;
      }
      p1 = particles[i];
      p2 = particles[j];
      bool collided =  CheckCollisionCircles((Vector2)p1.coord, p1.r, (Vector2)p2.coord, p2.r);
      if (collided){
        // todo
      }
    }
  }
}

int main(int argc, char *argv[]) {
  // CPU consumption control
  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
             "Smashing balls using physics, I guess");
  SetTargetFPS(60);
  init_particles();
  while (!WindowShouldClose() && !IsKeyPressed(KEY_Q)) {
    BeginDrawing();
    DrawFPS(10, 10);
    ClearBackground(BLACK);
    update_particles();
    collide_all_particles();
    draw_particles();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
