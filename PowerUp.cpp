#include "PowerUp.h"
#include "Paddle.h"
#include "Ball.h"
#include "raylib.h" 
#include <cstdlib>
#include <algorithm>

using namespace std;

extern int lives;

PowerUp::PowerUp(float x, float y, PowerUpType powerUpType) {
    posX = x;
    posY = y;
    width = 20;
    height = 20;
    speedY = 2.0f;
    isActive = false;
    isCollected = false;
    type = powerUpType;
    timer = 0;

    switch (type) {
    case EXPAND_PADDLE:
        duration = 10.0f;
        color = GREEN;
        break;
    case SHRINK_PADDLE:
        duration = 5.0f;
        color = RED;
        break;
    case EXTRA_LIFE:
        duration = 0.0f;
        color = PINK;
        break;
    default:
        duration = 10.0f;
        color = WHITE;
    }
}

void PowerUp::update(Paddle& paddle) {
    if (!isCollected) {
        posY += speedY;

        if (posY + height >= paddle.posY &&
            posY <= paddle.posY + paddle.height &&
            posX + width >= paddle.posX &&
            posX <= paddle.posX + paddle.width) {
            isCollected = true;
            
        }

        if (posY > GetScreenHeight()) {
            isCollected = true;
        }
    }

    if (isActive && duration > 0) {
        timer += GetFrameTime();
        if (timer >= duration) {
            isActive = false;
        }
    }
}

void PowerUp::draw() {
    if (!isCollected) {
        DrawRectangle((int)posX, (int)posY, (int)width, (int)height, color);
        DrawText(getTypeName(), (int)posX + 2, (int)posY + 5, 10, BLACK);
    }
}

void PowerUp::activate(Paddle& paddle) {
    switch (type) {
    case EXPAND_PADDLE:
        paddle.width = std::min(paddle.width * 2.0f, 150.0f); // limitiing the size of the paddle to make it not too big
   
        break;
    case SHRINK_PADDLE:
        paddle.width = std::max(paddle.width * 0.6f, 50.0f); //same for shrink 
        break;
    case EXTRA_LIFE:
        lives++;
        isActive = false;
        break;
    default:
        break;

    }
}

void PowerUp::deactivate(Paddle& paddle) {
    switch (type) {
    case EXPAND_PADDLE:
      
        paddle.width = std::max(paddle.width / 1.15f, 100.0f);
        printf("Paddle width after EXPAND: %f\n", paddle.width);
        break;
    case SHRINK_PADDLE:
       
        paddle.width = std::min(paddle.width / 0.85f, 100.0f);
        break;
    }
}

bool PowerUp::isExpired() {
    return isActive && timer >= duration;
}

const char* PowerUp::getTypeName() {
    switch (type) {
    case EXPAND_PADDLE: return "E";
    case SHRINK_PADDLE: return "S";
    case EXTRA_LIFE: return "+";
    default: return "?";
    }
}