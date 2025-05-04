#include "Ball.h"
#include "raylib.h"
#include "Brick.h"
#include <stdio.h>
#include "Paddle.h"

extern int score;
extern void spawnPowerUp(float x, float y);

Ball::Ball()
{
    posX = 0;
    posY = 0;
    speedX = 10;
    speedY = 10;
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
    }
}

void Ball::checkPaddleCollision(Paddle& paddle)
{
    if (posY + radius >= paddle.posY && posY + radius <= paddle.posY + paddle.height &&
        posX + radius >= paddle.posX && posX - radius <= paddle.posX + paddle.width) {

        if (posY + radius > paddle.posY) {
            posY = paddle.posY - radius;
        }

        speedY = -speedY;
    }
}

void Ball::checkBallBrickCollision(Brick* bricks[5][10]) {
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 10; col++) {
            Brick* brick = bricks[row][col];

            if (brick == nullptr || brick->isDestroyed) {
                continue;
            }

            if (posX + radius >= brick->posX &&
                posX - radius <= brick->posX + brick->width &&
                posY + radius >= brick->posY &&
                posY - radius <= brick->posY + brick->height) {

                if (brick->type == INDESTRUCTIBLE) {
                    bool hitFromSides = (posX - radius <= brick->posX || posX + radius >= brick->posX + brick->width);
                    bool hitFromTopBottom = (posY - radius <= brick->posY || posY + radius >= brick->posY + brick->height);

                    if (hitFromSides && hitFromTopBottom) {
                        speedX *= -1;
                        speedY *= -1;
                    }
                    else if (hitFromSides) {
                        speedX *= -1;
                    }
                    else if (hitFromTopBottom) {
                        speedY *= -1;
                    }
                    else {
                        speedY *= -1;
                    }
                    return;
                }

                brick->hit(bricks);

                if (!brick->isDestroyed) {
                    if (brick->type == DURABLE) score += 25;
                }
                else {
                    switch (brick->type) {
                    case STANDARD:
                        score += 100;
                        printf("Bonus: STANDARD brick destroyed. +100 points\n");
                        break;
                    case DURABLE:
                        score += 50;
                        printf("Bonus: DURABLE brick destroyed. +50 points\n");
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
                        printf("Bonus: Unknown brick type. No points awarded\n");
                        break;
                    }
                    if (brick->hasPowerUp) score += 50;
                }

                speedY *= -1;

                return;
            }
        }
    }
}

void Ball::draw()
{
    DrawCircle((int)posX, (int)posY, (int)radius, BLACK);
}