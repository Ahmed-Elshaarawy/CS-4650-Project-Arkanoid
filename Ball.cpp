#include "Ball.h"
#include "raylib.h"
#include "Brick.h"
#include <stdio.h>
#include "Paddle.h"
#include <cmath>
#include <iostream>

extern int score;
extern void spawnPowerUp(float x, float y);

extern float lastBrickHitTime;
extern int comboCount;
extern  float comboTimeWindow;


Ball::Ball()
{
    posX = 0;
    posY = 0;
    speedX = 100;
    speedY = 100;
    originalSpeedX = speedX;
    originalSpeedY = speedY;
    radius = 10;
    isLaunched = false;
}

void Ball::update(Paddle& paddle, Brick* bricks[5][10])
{
    if (!isLaunched) {
        posX = paddle.posX + paddle.width / 2;
        posY = paddle.posY - radius;
        return;
    }

    posX += speedX;
    posY += speedY;

    if (posX - radius <= 0) {
        posX = radius;
        speedX *= -1;
    }
    if (posX + radius >= GetScreenWidth()) {
        posX = GetScreenWidth() - radius;
        speedX *= -1;
    }
    if (posY - radius <= 0) {
        posY = radius;
        speedY *= -1;
    }

    checkBallBrickCollision(bricks);

    checkPaddleCollision(paddle);
}

void Ball::Launch() {
    if (!isLaunched) {
        isLaunched = true;
        speedX = 2;
        speedY = -2;
        originalSpeedX = speedX;
        originalSpeedY = speedY;

        comboCount = 0;
        lastBrickHitTime = 0.0f;
    }
}

void Ball::checkPaddleCollision(Paddle& paddle)
{
    float paddleDeflectionStrength = 5.0f;
    if (posY + radius >= paddle.posY && posY + radius <= paddle.posY + paddle.height &&
        posX + radius >= paddle.posX && posX - radius <= paddle.posX + paddle.width) {

            if (speedY > 0) 
            {
                speedY *= -1;
                float paddleCenterX = paddle.posX + paddle.width / 2.0f;
                float hitOffsetFromCenter = posX - paddleCenterX;
                float normalizedHitOffset = hitOffsetFromCenter / (paddle.width / 2.0f);
                speedX = normalizedHitOffset * paddleDeflectionStrength;

                if (posY + radius > paddle.posY) {
                    posY = paddle.posY - radius;
                }


                comboCount = 0;
                lastBrickHitTime = 0.0f;
        }
    }
}






void Ball::checkBallBrickCollision(Brick* bricks[5][10]) {
    float currentTime = GetTime();

    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 10; col++) {
            Brick* brick = bricks[row][col];

            if (brick == nullptr || brick->isDestroyed) {
                continue;
            }

            bool collision = (posX + radius >= brick->posX &&
                posX - radius <= brick->posX + brick->width &&
                posY + radius >= brick->posY &&
                posY - radius <= brick->posY + brick->height);

            if (collision) {
                float brickCenterX = brick->posX + brick->width / 2.0f;
                float brickCenterY = brick->posY + brick->height / 2.0f;
                float deltaX = posX - brickCenterX;
                float deltaY = posY - brickCenterY;

                bool hitHorizontal = (fabsf(deltaX) * brick->height > fabsf(deltaY) * brick->width);

                if (hitHorizontal) {
                    speedX *= -1;
                }
                else {
                    speedY *= -1;
                }

                if (brick->type == INDESTRUCTIBLE) {
                    return;
                }

                bool wasNotDestroyed = !brick->isDestroyed;

                brick->hit(bricks);

                if (brick->isDestroyed && wasNotDestroyed) {
                    if (currentTime - lastBrickHitTime <= comboTimeWindow) {
                        comboCount++;
                        score += comboCount * 10;
                        printf("Combo x%d! Bonus: %d\n", comboCount, comboCount * 10);
                    }
                    else {
                        comboCount = 1;
                        printf("New Combo x%d!\n", comboCount);
                    }
                    lastBrickHitTime = currentTime;

                    switch (brick->type) {
                    case STANDARD:
                        score += 100;
                        printf("Bonus: STANDARD brick destroyed. +100 points\n");
                        break;
                    case DURABLE:
                        score += 75;
                        printf("Bonus: DURABLE brick destroyed. +75 points\n");
                        break;
                    case POWERUP:
                        score += 150;
                        printf("Bonus: POWERUP brick destroyed. +150 points\n");
                        break;
                    case SPECIAL:
                        score += 200;
                        printf("Bonus: SPECIAL brick destroyed. +200 points\n");
                        break;
                    default:
                        printf("Bonus: Unknown brick type destroyed. No points awarded\n");
                        break;
                    }
                    if (brick->hasPowerUp || brick->type == POWERUP) {
                        score += 50;
                        printf("Bonus: Powerup collected. +50 points\n");
                    }
                }
                else if (wasNotDestroyed && !brick->isDestroyed && brick->type == DURABLE) {
                    score += 25;
                    printf("Bonus: DURABLE brick hit. +25 points\n");
                    comboCount = 0;
                    lastBrickHitTime = 0.0f;
                }

                return;
            }
        }
    }
}

void Ball::draw()
{
    DrawCircle((int)posX, (int)posY, (int)radius, BLACK);
}