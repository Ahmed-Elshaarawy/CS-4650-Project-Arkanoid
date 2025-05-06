#pragma once
#include "raylib.h"
#include "Paddle.h"
#include "Ball.h"

enum PowerUpType {
    EXPAND_PADDLE,
    SHRINK_PADDLE,
    SLOW_BALL,
    FAST_BALL,
    EXTRA_LIFE,
    POWER_UP_COUNT  // Used to get the total number of power-up types
};

class PowerUp {
public:
    float posX, posY;
    float width, height;
    float speedY;
    bool isActive;
    bool isCollected;
    PowerUpType type;
    float duration;
    float timer;
    Color color;

    PowerUp(float x, float y, PowerUpType powerUpType);
    void update(Paddle& paddle);
    void draw();
    void activate(Paddle& paddle);
    void deactivate(Paddle& paddle);
    bool isExpired();
    const char* getTypeName();
};
