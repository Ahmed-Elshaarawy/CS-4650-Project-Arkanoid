#pragma once
#include "Paddle.h"
#include "Brick.h"

class Ball {
public:
    float posX, posY;
    float speedX, speedY;
    float radius;
    bool isLaunched;
    float originalSpeedX;
    float originalSpeedY;

    Ball();
    void update(Paddle& paddle, Brick* bricks[5][10]);  
    void draw();
    void Launch();

private:
    void checkPaddleCollision(Paddle& paddle);
    void checkBallBrickCollision(Brick* bricks[5][10]); 

};
