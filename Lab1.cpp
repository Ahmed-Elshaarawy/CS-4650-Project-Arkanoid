#include "raylib.h"
#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"
#include <cstdio>
#include <vector>
#include "PowerUp.h"

const int screenWidth = 800;
const int screenHeight = 450;

const int rows = 5;
const int columns = 10;
Brick* bricks[rows][columns];

int lives = 3;
bool gameOver = false;
bool levelComplete = false;

int score = 0;
int highScore = 0;

vector<PowerUp*> activePowerUps;

// Forward declaration
void spawnPowerUp(float x, float y);

void initBricks() {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            BrickType brickType = static_cast<BrickType>(GetRandomValue(0, 3));
            bricks[row][col] = new Brick(col * 75 + 65, row * 40 + 60, brickType);

            // Randomly assign power-ups
            if (GetRandomValue(0, 100) < 30) {
                bricks[row][col]->hasPowerUp = true;
            }
        }
    }
}

bool checkWin() {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            if (!bricks[row][col]->isDestroyed && bricks[row][col]->type != INDESTRUCTIBLE) {
                return false;
            }
        }
    }
    return true;
}

void drawBricks() {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            bricks[row][col]->draw();
        }
    }
}

void resetBallPosition(Ball& ball1, Paddle& paddle) {
    ball1.isLaunched = false;
    ball1.posX = paddle.posX + paddle.width / 2 - ball1.radius;
    ball1.posY = paddle.posY - ball1.radius;
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

void updatePowerUps(Paddle& paddle, Ball& mainBall) {
    for (size_t i = 0; i < activePowerUps.size(); i++) {
        PowerUp* powerUp = activePowerUps[i];

        powerUp->update(paddle);

        if (powerUp->isCollected && powerUp->isActive) {
            powerUp->activate(paddle, mainBall);
        }

        if (powerUp->isExpired()) {
            powerUp->deactivate(paddle, mainBall);
            powerUp->isActive = false;
        }

        if (!powerUp->isActive && powerUp->isCollected) {
            delete powerUp;
            activePowerUps.erase(activePowerUps.begin() + i);
            i--;
        }
    }
}

void drawPowerUps() {
    for (size_t i = 0; i < activePowerUps.size(); i++) {
        activePowerUps[i]->draw();
    }
}

int main(void)
{
    InitWindow(screenWidth, screenHeight, "Raylib Brick Breaker Game");

    highScore = LoadHighScore();

    Ball ball1;
    Paddle paddle;

    initBricks();

    Texture2D heartTexture = LoadTexture("heart.png");

    SetTargetFPS(60);

    while (!WindowShouldClose() && lives > 0 && !gameOver)
    {
        if (!levelComplete) {
            ball1.update(paddle, bricks);
            paddle.update();
            updatePowerUps(paddle, ball1);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !ball1.isLaunched) {
                ball1.Launch();
            }

            if (ball1.posY >= screenHeight) {
                lives--;
                resetBallPosition(ball1, paddle);
            }

            levelComplete = checkWin();
        }

        if (lives <= 0) {
            gameOver = true;
            if (score > highScore) {
                SaveHighScore(score);
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

        if (gameOver) {
            DrawText("Game Over!", screenWidth / 2 - 70, screenHeight / 2, 40, RED);
        }

        if (levelComplete) {
            DrawText("Level Complete!", screenWidth / 2 - 120, screenHeight / 2, 40, GREEN);
        }

        DrawText(TextFormat("Score: %d", score), 10, 40, 20, DARKGRAY);
        DrawText(TextFormat("High Score: %d", highScore), 10, 70, 20, DARKGRAY);

        EndDrawing();
    }

    // Cleanup
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            delete bricks[row][col];
        }
    }

    for (auto powerUp : activePowerUps) {
        delete powerUp;
    }
    activePowerUps.clear();

    UnloadTexture(heartTexture);

    CloseWindow();
    return 0;
}
