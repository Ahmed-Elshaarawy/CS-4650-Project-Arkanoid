#include "Ball.h"
#include "raylib.h"
#include "Brick.h"


extern void spawnPowerUp(float x, float y);

extern int score;

void Ball::update(Paddle& paddle, Brick* bricks[5][10])
{
    if (!isLaunched) {
        posX = paddle.posX + paddle.width / 2 - radius;
        posY = paddle.posY - radius;
        return; // No need to continue if ball not launched
    }

    // Move the ball
    posX += speedX;
    posY += speedY;

    // Handle wall collisions
    if (posX > GetScreenWidth() || posX <= 0) speedX *= -1;
    if (posY <= 0) speedY *= -1;

 
    if (posY >= GetScreenHeight()) {
        posX = paddle.posX + paddle.width / 2 - radius;
        posY = paddle.posY - radius;
        speedX = 4;
        speedY = 4;
        originalSpeedX = speedX;
        originalSpeedY = speedY;
        isLaunched = false;
        return;
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
    if (posY + radius >= paddle.posY && posY + radius <= paddle.posY + paddle.height) {
        if (posX >= paddle.posX && posX <= paddle.posX + paddle.width) {
            speedY = -speedY;

            float paddleCenter = paddle.posX + paddle.width / 2;
            float hitPosition = posX - paddleCenter;

            if (posY + radius > paddle.posY) {
                posY = paddle.posY - radius;
            }
        }
    }
}

void Ball::checkBallBrickCollision(Brick* bricks[5][10]) {
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 10; col++) {
            Brick* brick = bricks[row][col];

            if (!brick->isDestroyed) {
                if (posX + radius >= brick->posX && posX - radius <= brick->posX + brick->width &&
                    posY + radius >= brick->posY && posY - radius <= brick->posY + brick->height) {

                    if (brick->type == INDESTRUCTIBLE) {
                        speedY = -speedY;
                        speedX = -speedX;
                        continue;  
                    }
                    if (brick->type == STANDARD) score += 100;
                    else if (brick->type == DURABLE) score += (brick->durability == 2 ? 50 : 25);
                    else if (brick->type == SPECIAL) score += 150;
                    if (brick->hasPowerUp) score += 50;



                    brick->hit();
                    speedY = -speedY;
                }
            }
        }
    }
}

void Ball::draw()
{
    DrawCircle(posX, posY, radius, BLACK);
}

Ball::Ball()
{
    posX = 2;
    posY = 3;
    speedX = 4;
    speedY = 4;
    originalSpeedX = speedX;
    originalSpeedY = speedY;
    radius = 10;
    isLaunched = false;

}
