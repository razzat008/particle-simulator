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
#include "raymath.h"
#include <math.h>
#include <stdbool.h>
#include <time.h>

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

#define BALLS 30
#define MAX_SPEED 10.0f
#define PENETRATION_SLOP 0.01f

typedef struct {
  float x;
  float y;
} Coordinates;

// typedef enum { LEFT, RIGHT, TOP, BOTTOM } COLLIDED;

typedef struct {
  Coordinates coord;
  float r;
  Vector2 velocity;
  Color color;
  float mass;
  // COLLIDED collided;
} Particle;

typedef struct {
  float left;
  float top;
  float bottom;
  float right;
} Bounds;

Particle particles[BALLS];

void update_particle(Particle *);
void update_particles();
void draw_particle(Particle *);
void draw_particles();
void repulse(Particle *, Particle *);

// draw circle
void draw_particle(Particle *particle) {
  float mass = (float)1000 * 4 / 3 * PI * (powf(particle->r, 3));
  particle->mass = mass;
  DrawCircle(particle->coord.x, particle->coord.y, particle->r,
             particle->color);
}

void draw_particles() {
  for (int i = 0; i < BALLS; i++) {
    draw_particle(particles + i);
  }
}

// bounding particles within the bounds of observable universe
void update_particle(Particle *particle) {
  particle->coord.x += particle->velocity.x;
  particle->coord.y += particle->velocity.y;

  float x = particle->coord.x;
  float y = particle->coord.y;
  float radius = particle->r;

  if (x - radius < 0) { // left wall

    particle->coord.x = radius;
    particle->velocity.x = -particle->velocity.x;
  }
  if (x + radius > WINDOW_WIDTH) { // right wall

    particle->coord.x = WINDOW_WIDTH - radius;
    particle->velocity.x = -particle->velocity.x;
  }
  if (y + radius > WINDOW_HEIGHT) { // bottom wall

    particle->coord.y = WINDOW_HEIGHT - radius;
    particle->velocity.y = -particle->velocity.y;
  }
  if (y - radius < 0) { // top wall

    particle->coord.y = radius;
    particle->velocity.y = -particle->velocity.y;
  }
}

void update_particles() {
  for (int i = 0; i < BALLS; i++) {
    update_particle(particles + i);
  }
}

void init_particles() {
  SetRandomSeed(time(NULL));
  Color colorarry[] = {WHITE, BLACK, RED, PURPLE, GREEN, YELLOW};
  int arraysize = sizeof(colorarry) / sizeof(colorarry[0]);
  for (int i = 0; i < BALLS; i++) {
    // lets generate random colors lesssgooooooo
    int randomIndex = GetRandomValue(0, arraysize - 1);
    particles[i] = (Particle){
        {GetRandomValue(0, WINDOW_WIDTH), GetRandomValue(0, WINDOW_HEIGHT)},
        GetRandomValue(5, 30), // radius
        {
            GetRandomValue(-5, 5),
            GetRandomValue(-5, 5),
        }, // velocity.y
        colorarry[randomIndex]};
  }
}

// verryyy shitty logic, takes a lot of computation
// will improve this when I get bored again
void collide_all_particles() {
  Particle *p1, *p2;
  for (int i = 0; i < BALLS; i++) {
    for (int j = i + 1; j < BALLS; j++) {
      p1 = &particles[i];
      p2 = &particles[j];
      Vector2 c1 = {p1->coord.x, p1->coord.y};
      Vector2 c2 = {p2->coord.x, p2->coord.y};
      bool collided = CheckCollisionCircles(c1, p1->r, c2, p2->r);
      if (collided) {
        repulse(p1, p2);
      }
    }
  }
}

// perfect elastic collision
// using conservation of kinetic energy & conservation of momentum
void repulse(Particle *p1, Particle *p2) {

  // position of each neighbouring particles
  Vector2 pos1 = {p1->coord.x, p1->coord.y};
  Vector2 pos2 = {p2->coord.x, p2->coord.y};

  // mass of each neighbouring particles
  float m1 = p1->mass;
  float m2 = p2->mass;

  Vector2 delta = Vector2Subtract(pos1, pos2);
  float dist = Vector2Length(delta);
  if (dist == 0.0) {
    return;
  }
  /*      r1        r2
         <-->      <-->

      (  O  )      (  O  )
         |            |
         |<-- dist -->|

   dist > r1 + r2
   overlap = (r1 + r2) - dist < 0
  */

  float overlap = (p1->r + p2->r) - dist;

  // letting things overlap to some extent (PENETRATION_SLOP)
  if (overlap > PENETRATION_SLOP) {
    // unit normal vector(scaling down)
    Vector2 normal = Vector2Scale(delta, 1.0f / dist);

    // push them apart proportional to mass
    float totalMass = m1 + m2;

    p1->coord.x += normal.x * (overlap * (m2 / totalMass));
    p1->coord.y += normal.y * (overlap * (m2 / totalMass));

    p2->coord.x -= normal.x * (overlap * (m1 / totalMass));
    p2->coord.y -= normal.y * (overlap * (m1 / totalMass));
  }

  // veclocity of each neighbouring particles
  Vector2 v1 = p1->velocity;
  Vector2 v2 = p2->velocity;

  Vector2 diffvec = Vector2Subtract(v1, v2);
  Vector2 positionvectors = Vector2Subtract(pos1, pos2);

  // checking if two particles are approaching or not
  if (Vector2DotProduct(diffvec, positionvectors) >= 0) {
    return;
  }

  float distSq = Vector2DotProduct(positionvectors, positionvectors);
  if (distSq == 0.0f)
    return;

  // new velocity for particle-1
  float dot1 = Vector2DotProduct(diffvec, positionvectors);
  float scalar = (2.0f * m2 / (m1 + m2)) * (dot1 / distSq);

  Vector2 newv1 = Vector2Subtract(v1, Vector2Scale(positionvectors, scalar));

  // new velocity for particle-2
  Vector2 diffvec2 = Vector2Subtract(v2, v1);
  Vector2 positionvectors2 = Vector2Subtract(pos2, pos1);

  float dot2 = Vector2DotProduct(diffvec2, positionvectors2);
  float scalar2 = (2.0f * m1 / (m1 + m2)) * (dot2 / distSq);

  Vector2 newv2 = Vector2Subtract(v2, Vector2Scale(positionvectors2, scalar2));

  // new velocity after collision
  p1->velocity = Vector2ClampValue(newv1, 0, MAX_SPEED);
  p2->velocity = Vector2ClampValue(newv2, 0, MAX_SPEED);
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
    draw_particles();
    ClearBackground(BLACK);
    update_particles();
    collide_all_particles();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
