#pragma once

class Paddle {
public:
    float posX, posY;
    float speedX;
    float width, height;
    

    Paddle();
    void update();
    void draw();
};
