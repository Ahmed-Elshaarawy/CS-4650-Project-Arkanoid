#include "Paddle.h"
#include "raylib.h"

Paddle::Paddle() {
    posX = 400;  
    posY = 400;  
    width = 100; 
    height = 20; 
    speedX = 5;  
}

void Paddle::update() {
    
    posX = GetMouseX() - width / 2;  

    if (IsKeyDown(KEY_LEFT)) {
        posX -= speedX;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        posX += speedX;
    }

    
    if (posX < 0) {
        posX = 0;
    }
    if (posX + width > GetScreenWidth()) {
        posX = GetScreenWidth() - width;
    }
}

void Paddle::draw() {
    DrawRectangle(posX, posY, width, height, BLUE);  
}
