#include "Brick.h"
#include "raylib.h"
#include "PowerUp.h"

extern void spawnPowerUp(float x, float y);

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
        color = MAGENTA;
        break;
    case DURABLE:
        durability = 2;
        color = RED;
        break;
    case POWERUP:
        durability = 1;
        hasPowerUp = true;
        color = BLUE;
        break;
    case INDESTRUCTIBLE:
        durability = -1;
        color = GRAY;
        break;
    case SPECIAL:
        durability = 1;
        hasPowerUp = false;
        color = BLACK;
        break;
    default:
        durability = 1;
        color = WHITE;
        break;
    }
}

void Brick::draw()
{
    if (isDestroyed) {
        return;
    }

    if (type == DURABLE) {
        switch (durability) {
        case 2: color = ORANGE; break;
        case 1: color = YELLOW; break;
        }
    }

    DrawRectangle((int)posX, (int)posY, (int)width, (int)height, color);
}

void Brick::hit(Brick* bricks[5][10]) {
    if (type == INDESTRUCTIBLE) {
        return;
    }

    durability--;

    if (durability <= 0) {
        isDestroyed = true;

        if (type == POWERUP || hasPowerUp) {
            activateEffect();
        }
        if (type == SPECIAL) {
            specialEffect(bricks);
        }
    }
}

void Brick::activateEffect() {
    if (hasPowerUp || type == POWERUP) {
        spawnPowerUp(posX + width / 2, posY + height / 2);
    }
}

void Brick::specialEffect(Brick* bricks[5][10]) {
    int centerRow = (int)((posY - 60) / 40);
    int centerCol = (int)((posX - 65) / 75);

    for (int r = centerRow - 1; r <= centerRow + 1; ++r) {
        for (int c = centerCol - 1; c <= centerCol + 1; ++c) {
            if (r >= 0 && r < 5 && c >= 0 && c < 10) {
                Brick* neighborBrick = bricks[r][c];
                if (neighborBrick != nullptr) {
                    if (!neighborBrick->isDestroyed && neighborBrick->type != INDESTRUCTIBLE) {
                        neighborBrick->isDestroyed = true;

                        if (neighborBrick->hasPowerUp || neighborBrick->type == POWERUP) {
                            neighborBrick->activateEffect();
                        }
                    }
                }
            }
        }
    }
}