#pragma once
#include "raylib.h" 


enum BrickType { STANDARD, DURABLE, INDESTRUCTIBLE, SPECIAL, POWERUP  };


class Brick
{
public:
    float posX, posY;      
    float width, height;    
    BrickType type;         
    bool isDestroyed;    
    int durability;
    Color color;
    bool hasPowerUp;

    
    Brick(float x, float y, BrickType brickType);

    void draw();
    void hit(Brick* bricks[5][10]);
    void activateEffect();
    void specialEffect(Brick* bricks[5][10]);
};
