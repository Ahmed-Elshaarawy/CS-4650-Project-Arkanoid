#include "PowerUp.h"
#include "Paddle.h"
#include "Ball.h"
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
       /* case SLOW_BALL:
            duration = 10.0f;
            color = BLUE;
            break;
        case FAST_BALL:
            duration = 8.0f;
            color = ORANGE;
            break;*/
        case EXTRA_LIFE:
            duration = 0.0f; // Instant effect
            color = PINK;
            break;
        default:
            duration = 10.0f;
            color = WHITE;
    }
}

void PowerUp::update(Paddle& paddle) {
    if (!isCollected) {
        // Power-up falls down the screen
        posY += speedY;
        
        // Check if power-up is collected by paddle
        if (posY + height >= paddle.posY && 
            posY <= paddle.posY + paddle.height &&
            posX + width >= paddle.posX && 
            posX <= paddle.posX + paddle.width) {
            isCollected = true;
            isActive = true;
        }
        
        // Check if power-up falls off screen
        if (posY > GetScreenHeight()) {
            isCollected = true; // Remove from game
        }
    }
    
    // Update timer for active power-ups
    if (isActive && duration > 0) {
        timer += GetFrameTime();
        if (timer >= duration) {
            isActive = false;
        }
    }
}

void PowerUp::draw() {
    if (!isCollected) {
        DrawRectangle(posX, posY, width, height, color);
        
        // Draw a letter to indicate power-up type
        DrawText(getTypeName(), posX + 2, posY + 5, 10, BLACK);
    }
}

void PowerUp::activate(Paddle& paddle, Ball& ball) {
    switch (type) {
        case EXPAND_PADDLE:
            paddle.width = std::min(paddle.width * 1.15f, 150.0f); // 15% increase, max 150px
            break;
        case SHRINK_PADDLE:
            paddle.width = std::max(paddle.width * 0.85f, 50.0f); // 15% decrease, min 50px
            break;
        //case SLOW_BALL:
        //    ball.speedX *= 0.85f; // 15% slower
        //    ball.speedY *= 0.85f;
        //    break;
        //case FAST_BALL:
        //    ball.speedX *= 1.15f; // 15% faster
        //    ball.speedY *= 1.15f;
        //    break;
        case EXTRA_LIFE:
            lives++;
            isActive = false; // Instant effect
            break;
    }
}

void PowerUp::deactivate(Paddle& paddle, Ball& ball) {
    switch (type) {
        case EXPAND_PADDLE:
            paddle.width = std::min(paddle.width / 1.15f, 100.0f); // Reset to original size
            break;
        case SHRINK_PADDLE:
            paddle.width = std::max(paddle.width / 0.85f, 100.0f); // Reset to original size
            break;
        //case SLOW_BALL:
        //    ball.speedX /= 0.85f; // Reset to original speed
        //    ball.speedY /= 0.85f;
        //    break;
        //case FAST_BALL:
        //    ball.speedX /= 1.15f; // Reset to original speed
        //    ball.speedY /= 1.15f;
        //    break;
    }
}

bool PowerUp::isExpired() {
    return isActive && timer >= duration;
}

const char* PowerUp::getTypeName() {
    switch (type) {
        case EXPAND_PADDLE: return "E";
        case SHRINK_PADDLE: return "S";
       /* case SLOW_BALL: return "SL";
        case FAST_BALL: return "F";*/
        case EXTRA_LIFE: return "+";
        default: return "?";
    }
}
