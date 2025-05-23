#include "raylib.h"
#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"
#include <cstdio>
#include <vector>
#include "PowerUp.h"
#include <algorithm>

using namespace std;

const int screenWidth = 800;
const int screenHeight = 450;

const int rows = 5;
const int columns = 10;
Brick* bricks[rows][columns];

int lives = 3;
bool gameOver = false;
bool winning = false;

int score = 0;
int highScore = 0;

vector<PowerUp*> activePowerUps;

float lastBrickHitTime = 0.0f;
int comboCount = 0;
float comboTimeWindow = 0.5f;

//float levelStartTime = 0.0f;


void spawnPowerUp(float x, float y);
void initBricks();
bool checkWin();
void drawBricks();
void resetBallPosition(Ball& ball1, Paddle& paddle);
void SaveHighScore(int score);
int LoadHighScore();
void updatePowerUps(Paddle& paddle);
void drawPowerUps();


void initBricks() {
    float startX = 65;
    float startY = 60;
    float paddingX = 75;
    float paddingY = 40;

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            BrickType brickType = static_cast<BrickType>(GetRandomValue(0, 4));

            bricks[row][col] = new Brick(startX + col * paddingX, startY + row * paddingY, brickType);

            if (brickType != POWERUP) {
                if (GetRandomValue(0, 100) < 30) {
                    bricks[row][col]->hasPowerUp = true;
                }
            }
        }
    }
 
    comboCount = 0;
    lastBrickHitTime = 0.0f;
}

bool checkWin() {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            Brick* brick = bricks[row][col];
            if (brick != nullptr && !brick->isDestroyed && brick->type != INDESTRUCTIBLE) {
                return false;
            }
        }
    }
    return true;
}

void drawBricks() {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            if (bricks[row][col] != nullptr) {
                bricks[row][col]->draw();
            }
        }
    }
}

void resetBallPosition(Ball& ball1, Paddle& paddle) {
    ball1.isLaunched = false;
    ball1.posX = paddle.posX + paddle.width / 2;
    ball1.posY = paddle.posY - ball1.radius;
    ball1.speedX = ball1.originalSpeedX;
    ball1.speedY = ball1.originalSpeedY;
    comboCount = 0;
    lastBrickHitTime = 0.0f;
}

void SaveHighScore(int score) {
    FILE* file = nullptr;
    fopen_s(&file, "highscore.txt", "w");
    if (file) {
        fprintf(file, "%d", score);
        fclose(file);
    }
}

int LoadHighScore() {
    FILE* file = nullptr;
    fopen_s(&file, "highscore.txt", "r");
    int score = 0;
    if (file) {
        fscanf_s(file, "%d", &score);
        fclose(file);
    }
    return score;
}

void spawnPowerUp(float x, float y) {
    PowerUpType type = static_cast<PowerUpType>(GetRandomValue(0, POWER_UP_COUNT - 1));
    PowerUp* powerUp = new PowerUp(x, y, type);
    activePowerUps.push_back(powerUp);
}

void updatePowerUps(Paddle& paddle) {
    for (size_t i = 0; i < activePowerUps.size(); ++i) {
        PowerUp* powerUp = activePowerUps[i];

        powerUp->update(paddle);

        if (powerUp->isCollected && !powerUp->isActive) {
            powerUp->activate(paddle);
            powerUp->isActive = true;
            printf("Activating power-up type: %d\n", powerUp->type);
        }

        if (powerUp->isActive && powerUp->isExpired()) {
            powerUp->deactivate(paddle);
            powerUp->isActive = false;
        }

        if (!powerUp->isActive && powerUp->isCollected) {
            delete powerUp;
            activePowerUps.erase(activePowerUps.begin() + i);
            --i;
        }
    }
}

void drawPowerUps() {
    for (size_t i = 0; i < activePowerUps.size(); i++) {
        if (activePowerUps[i] != nullptr) {
            activePowerUps[i]->draw();
        }
    }
}

int main(void)
{
    InitWindow(screenWidth, screenHeight, "ARkanoid");

    highScore = LoadHighScore();

    Image icon = LoadImage("icon.png");
    if (icon.data) {
        SetWindowIcon(icon);
        UnloadImage(icon);
    }
    else {
        printf("Failed to load window icon: icon.png\n");
    }

    Ball ball1;
    Paddle paddle;

    initBricks();

    Texture2D heartTexture = LoadTexture("heart.png");

    if (heartTexture.id == 0) {
        printf("Failed to load heart texture: heart.png\n");
        return -1;
    }
    heartTexture.width = 20;
    heartTexture.height = 20;

    SetTargetFPS(60);

    while (!WindowShouldClose() && !gameOver)
    {
        if (!winning) {
            ball1.update(paddle, bricks);
            paddle.update();
            updatePowerUps(paddle);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !ball1.isLaunched) {
                ball1.Launch();
            }

            if (ball1.posY - ball1.radius >= screenHeight) {
                lives--;
                if (lives <= 0) {
                    gameOver = true;
                }
                else {
                    resetBallPosition(ball1, paddle);
                }
            }

            winning = checkWin();
            if (score > highScore) {
                highScore = score;
                SaveHighScore(highScore);
            }

         
        }
        else {
            if (IsKeyPressed(KEY_SPACE)) {
                gameOver = true;
            }
        }

        if (lives <= 0) {
            gameOver = true;
            if (score > highScore) {
                highScore = score;
                SaveHighScore(highScore);
            }
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        drawBricks();
        drawPowerUps();
        ball1.draw();
        paddle.draw();

        for (int i = 0; i < lives; i++) {
            DrawTexture(heartTexture, 10 + i * (heartTexture.width + 5), 10, WHITE);
        }

        DrawText(TextFormat("Score: %d", score), 10, 40, 20, DARKGRAY);
        DrawText(TextFormat("High Score: %d", highScore), 10, 70, 20, DARKGRAY);

        if (comboCount > 0) {
            DrawText(TextFormat("Combo: %d", comboCount), screenWidth - 150, 10, 20, DARKGRAY);
        }

        if (gameOver) {
            DrawText("Game Over!", screenWidth / 2 - MeasureText("Game Over!", 40) / 2, screenHeight / 2 - 20, 40, RED);
        }
        else if (winning) {
            DrawText("YOU WON !", screenWidth / 2 - MeasureText("YOU WON !", 40) / 2, screenHeight / 2 - 20, 40, GREEN);
        
        }

        EndDrawing();
    }

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            if (bricks[row][col] != nullptr) {
                delete bricks[row][col];
                bricks[row][col] = nullptr;
            }
        }
    }

    for (auto powerUp : activePowerUps) {
        if (powerUp != nullptr) {
            delete powerUp;
        }
    }

    activePowerUps.clear();

    UnloadTexture(heartTexture);

    CloseWindow();

    return 0;
}