#pragma once
#include "raylib.h" 


enum BrickType { STANDARD, DURABLE, INDESTRUCTIBLE, SPECIAL, POWERUP };


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
    void hit();
    void activateEffect();
};
