#include "Brick.h"

Brick::Brick(float x, float y, BrickType brickType)
{
    posX = x;
    posY = y;
    width = 50;
    height = 20;
    type = brickType;
    isDestroyed = false;
    hasPowerUp = false;

    switch (type) {
    case STANDARD:
        durability = 1;
        hasPowerUp = true;
        color = MAGENTA;
        break;
    case DURABLE:
        durability = 2;
        color = RED;
        break;
    case SPECIAL:
        durability = 1;
        hasPowerUp = true;
        color = BLUE;
        break;
    case INDESTRUCTIBLE:
        durability = -1;
        color = GRAY;
        break;
    }
}

void Brick::draw()
{
    if (isDestroyed) return;

    if (type == DURABLE) {
        switch (durability) {
        case 2: color = ORANGE; break;
        case 1: color = YELLOW; break;
        }
    }

    DrawRectangle(posX, posY, width, height, color);
}

void Brick::hit() {
    if (type == INDESTRUCTIBLE) return;

    if (type == DURABLE) {
        durability--;
        if (durability <= 0) {
            isDestroyed = true;
        }
    }
    else if (type == STANDARD) {
        isDestroyed = true;
    }
    else if (type == SPECIAL) {
        isDestroyed = true;
        activateEffect();
    }
}

void Brick::activateEffect() {
    if (hasPowerUp) {
        // Trigger your power-up or effect here.
    }
}
