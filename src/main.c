#include "raylib.h"
#include <math.h>
#include "raymath.h"

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 700

#define MAX_BALL_COUNT 5
#define RADII_SCALE 20

/* Balls struct */
typedef struct {
    Vector2 positions[MAX_BALL_COUNT];
    Vector2 velocities[MAX_BALL_COUNT];
    float masses[MAX_BALL_COUNT];
    float radii[MAX_BALL_COUNT];
} Balls;

void setValues(Balls *balls);
void checkEdgeCollision(Balls *ball, int idx);
void checkCollision(Balls *balls);

int main(void) {

    //window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Collision 2D");
    SetTargetFPS(100);

    Balls balls = {0};
    setValues(&balls); // set initial values of each ball

    while (!WindowShouldClose()) {

        float dt = GetFrameTime();

        // let balls move
        for(int i = 0; i < MAX_BALL_COUNT; i++) {
            Vector2 vel = balls.velocities[i];
            vel.x = vel.x * dt * 50;
            vel.y = vel.y * dt * 50;

            balls.positions[i] = Vector2Add(balls.positions[i], vel);
            checkEdgeCollision(&balls, i);
        }

        checkCollision(&balls);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            for (int i = 0; i < MAX_BALL_COUNT; i++) {
                DrawCircleV(balls.positions[i], balls.radii[i], i == 0 ? RED : SKYBLUE);
            }
        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}

void setValues(Balls *balls) {
    for (int i = 0; i < MAX_BALL_COUNT; i++) {

        // apply initial velocities to each component
        balls->velocities[i].x = GetRandomValue(10, 30);
        balls->velocities[i].y = GetRandomValue(10, 30);

        // mass
        balls->masses[i] = GetRandomValue(2, 10);

        // radius -- apply according to it's mass
        balls->radii[i] = sqrtf(balls->masses[i]) * RADII_SCALE;

        // position
        balls->positions[i].x = GetRandomValue(balls->radii[i], SCREEN_WIDTH - balls->radii[i]);
        balls->positions[i].y = GetRandomValue(balls->radii[i], SCREEN_HEIGHT - balls->radii[i]);
    }
}

void checkEdgeCollision(Balls *ball, int idx) {

    // height and width of screen
    int h = GetScreenHeight();
    int w = GetScreenWidth();

    // for top
    if(ball->positions[idx].y < ball->radii[idx]) {
        ball->positions[idx].y = ball->radii[idx];
        ball->velocities[idx].y *= -1;
    }

    // for bottom
    if(ball->positions[idx].y > h - ball->radii[idx]) {
        ball->positions[idx].y = h - ball->radii[idx];
        ball->velocities[idx].y *= -1;
    }

    // for left
    if(ball->positions[idx].x < ball->radii[idx]) {
        ball->positions[idx].x = ball->radii[idx];
        ball->velocities[idx].x *= -1;
    }

    // for right
    if(ball->positions[idx].x > w - ball->radii[idx]) {
        ball->positions[idx].x = w - ball->radii[idx];
        ball->velocities[idx].x *= -1;
    }
}

void checkCollision(Balls *balls) {

    Vector2 normal = {0};

    for(int i = 0; i < MAX_BALL_COUNT; i++) {

        // for ballA
        Vector2 ballAPos = balls->positions[i];
        Vector2 ballAVel = balls->velocities[i];
        float ballARadius = balls->radii[i];
        float m1 = balls->masses[i];

        for (int j = i+1; j < MAX_BALL_COUNT; j++) {

            // for ballB
            Vector2 ballBPos = balls->positions[j];
            Vector2 ballBVel = balls->velocities[j];
            float ballBRadius = balls->radii[j];
            float m2 = balls->masses[j];

            // let's check if the ball will collide
            Vector2 n = Vector2Subtract(ballBPos, ballAPos);
            float dist = Vector2Length(n);

            if(dist < ballARadius + ballBRadius) {

                // if they collide let's move them apart
                float depth = ballARadius + ballBRadius - dist;

                if(dist == 0) {
                    normal.x = 1.0f;
                    normal.y = 0.0f;
                } else {
                    normal = Vector2Normalize(n);
                }

                float moveDist = depth * 0.5;

                // move ballA opposite to the normal vector
                balls->positions[i].x -= normal.x * moveDist;
                balls->positions[i].y -= normal.y * moveDist; 

                // move ballB towards the normal vector
                balls->positions[j].x += normal.x * moveDist;
                balls->positions[j].y += normal.y * moveDist; 

                // correct dist
                dist = ballARadius + ballBRadius;

                // tangent vector
                Vector2 tangent = {-normal.y, normal.x};

                // project vectors into normal and tangent components
                float v1n = Vector2DotProduct(normal, ballAVel);
                float v1t = Vector2DotProduct(tangent, ballAVel);
                float v2n = Vector2DotProduct(normal, ballBVel);
                float v2t = Vector2DotProduct(tangent, ballBVel);

                float v1nPrime = (v1n * (m1 - m2) + 2*m2*v2n) / (m1 + m2);
                float v2nPrime = (v2n * (m2 - m1) + 2*m1*v1n) / (m1 + m2);

                Vector2 v1n_ = Vector2Scale(normal, v1nPrime);
                Vector2 v1t_ = Vector2Scale(tangent, v1nPrime);
                Vector2 v2n_ = Vector2Scale(normal, v2nPrime);
                Vector2 v2t_ = Vector2Scale(tangent, v2nPrime);

                balls->velocities[i] = Vector2Add(v1n_, v1t_);
                balls->velocities[j] = Vector2Add(v2n_, v2t_);
            }
        }
    }
}

/*
    So we have leanrned how to do 2D collision!!! GGS!!!

    THEY ARE GOING CRAZYY!!!! YEEE!!!

    BYEE!!!!!!

    CODE WILL BE POSTED IN DESCRIPTION!!!

    VYEEEEEEE!!!!!!!!!!!
*/