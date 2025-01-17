#include "raylib/include/raylib.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string>
#include <algorithm>
#include <vector>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#define PLAYERS_MAX_LIVES 5

typedef struct Player {
    Vector2 position;
    Vector2 size;
    int life;
} Player;

typedef struct Ball {
    Vector2 position;
    Vector2 speed;
    int radius;
    int direction;
} Ball;

typedef struct Marker {
    int markerPlayer1;
    int markerPlayer2;
} Marker;

static Player player1 = { 0 };
static Player player2 = { 0 };
static Ball ball = { 0 };
static Marker marker = { 0, 0 };
static std::vector<Ball> balls;
int multiplicatorspeed = 5;
int curselected = 0;
int selectedScreen = 0;
int spawnedball = 0;
char markerText[200];
std::string mode;
bool gamestarted = false;
bool inmainmenu = true;
bool isfullscreen = false;
std::string screenmodes[] = {
    "Window",
    "Fullscreen"
};
Texture2D oneBall;
Color textColor1 = GRAY;
Color textColor2 = GRAY;
Color textColorMENU1 = GRAY;
Color textColorMENU2 = GRAY;
Color textColorMENU3 = GRAY;

static void UpdateMenu(void);
static void UpdateMenuDrawFrame(void);
static void DrawMenu(void);
static void InitGame(void);
static void UpdateGame(void);
static void DrawGame(void);
static void UpdateGameDrawFrame(void);
static void resetgame(void);
static void addNewBall(const Ball& originalBall);

int main(void)
{
    InitWindow(1000, 700, "Cppong");

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateGameDrawFrame, 60, 1);
#else
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (gamestarted)
        {
            UpdateGameDrawFrame();
        }
        else
        {
            UpdateMenuDrawFrame();
        }
    }
#endif

    CloseWindow();
    return 0;
}

void InitGame(void)
{
    player1.position = Vector2{ 35, 350 };
    player1.size = Vector2{ 30, 230 };
    player1.life = PLAYERS_MAX_LIVES;

    player2.position = Vector2{ 965, 350 };
    player2.size = Vector2{ 30, 230 };
    player2.life = PLAYERS_MAX_LIVES;

    ball.position = Vector2{ 500, 350 };
    ball.speed = Vector2{ 0, 0 };
    ball.radius = 7;
    ball.direction = 0;

    resetgame();
}

void resetgame(void)
{
    multiplicatorspeed = 5;
    player1.position.y = 350;
    player2.position.y = 350;

    balls.clear();
    Ball initialBall = { Vector2{ 500.0f, 350.0f }, Vector2{ static_cast<float>(multiplicatorspeed), 0.0f }, 7, 0 };
    balls.push_back(initialBall);

    sprintf(markerText, "Marker Player1: %d, Marker Player2: %d",
        marker.markerPlayer1,
        marker.markerPlayer2);
}

void UpdateGame(void)
{
    if (player1.position.y > 90)
        if (IsKeyDown(KEY_W)) player1.position.y -= 10;

    if (player1.position.y < 600)
        if (IsKeyDown(KEY_S)) player1.position.y += 10;

    if (player2.position.y > 90)
        if (IsKeyDown(KEY_UP)) player2.position.y -= 10;

    if (player2.position.y < 600)
        if (IsKeyDown(KEY_DOWN)) player2.position.y += 10;

    for (Ball& ball : balls)
    {
        if (CheckCollisionCircleRec(ball.position, ball.radius,
            Rectangle{ player1.position.x - player1.size.x / 2, player1.position.y - player1.size.y / 2, player1.size.x, player1.size.y }))
        {
            if (mode == "multiball" && spawnedball == 0)
            {
                spawnedball = 1;
                addNewBall(ball);  // Genera una nueva bola solo una vez
            }

            ball.speed.y = GetRandomValue(-3, 3);
            ball.direction = 0;
            multiplicatorspeed += 1;
            ball.speed.x = multiplicatorspeed;
        }

        if (CheckCollisionCircleRec(ball.position, ball.radius,
            Rectangle{ player2.position.x - player2.size.x / 2, player2.position.y - player2.size.y / 2, player2.size.x, player2.size.y }))
        {
            if (mode == "multiball" && spawnedball == 1)
            {
                spawnedball = 0;
                addNewBall(ball);  // Genera una nueva bola solo una vez
            }

            ball.speed.x = multiplicatorspeed;
            ball.speed.y = GetRandomValue(-3, 3);
            ball.direction = 1;
            multiplicatorspeed += 1;
        }

        ball.position.x += ball.direction == 0 ? ball.speed.x : -ball.speed.x;
        ball.position.y += ball.speed.y;

        if (ball.position.y < 0 || ball.position.y > 700)
            ball.speed.y *= -1;

        if (ball.position.x > 1000)
        {
            marker.markerPlayer1 += 1;
            resetgame();
            break;
        }

        if (ball.position.x < 0)
        {
            marker.markerPlayer2 += 1;
            resetgame();
            break;
        }
    }
}

void addNewBall(const Ball& originalBall)
{
    Ball newBall = originalBall;
    newBall.speed.x = multiplicatorspeed - 0.8;
    newBall.speed.y = GetRandomValue(-3, 3);
    newBall.position.x += GetRandomValue(-3, 3);
    newBall.position.y += GetRandomValue(-3, 3);
    balls.push_back(newBall);
}

void DrawGame(void)
{
    BeginDrawing();

    ClearBackground(BLACK);

    DrawText(markerText, GetScreenWidth() / 2 - MeasureText(markerText, 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);
    DrawText("Player 1: WASD, Player 2: ARROW KEYS", GetScreenWidth() / 2 - MeasureText("Player 1: WASD, Player 2: ARROW KEYS", 10) / 2, 650, 10, GRAY);

    if (mode == "multiball")
    {
        player1.size = Vector2{ 30, 500 };
        player2.size = Vector2{ 30, 500 };
    }

    DrawRectangle(player1.position.x - player1.size.x / 2, player1.position.y - player1.size.y / 2, player1.size.x, player1.size.y, RAYWHITE);
    DrawRectangle(player2.position.x - player2.size.x / 2, player2.position.y - player2.size.y / 2, player2.size.x, player2.size.y, RAYWHITE);

    for (const Ball& ball : balls)
        DrawCircleV(ball.position, ball.radius, RAYWHITE);

    EndDrawing();
}

void UpdateGameDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}

void UpdateMenu(void)
{
    if (IsKeyPressed(KEY_W))
    {
        if (inmainmenu)
        {
            curselected = curselected > 0 ? curselected - 1 : 2;
        }
        else
        {
            curselected = curselected > 0 ? curselected - 1 : 1;
        }
    }

    if (IsKeyPressed(KEY_S))
    {
        if (inmainmenu)
        {
            curselected = curselected < 2 ? curselected + 1 : 0;
        }
        else
        {
            curselected = curselected < 1 ? curselected + 1 : 0;
        }
    }

    if (inmainmenu)
    {
        textColorMENU1 = curselected == 0 ? WHITE : GRAY;
        textColorMENU2 = curselected == 1 ? WHITE : GRAY;
        textColorMENU3 = curselected == 2 ? WHITE : GRAY;
    }
    else
    {
        textColor1 = curselected == 0 ? WHITE : GRAY;
        textColor2 = curselected == 1 ? WHITE : GRAY;
    }

    if (IsKeyReleased(KEY_BACKSPACE))
    {
        if (inmainmenu)
        {
            CloseWindow();
        }
        if (!inmainmenu && !gamestarted)
        {
            inmainmenu = true;
        }
        if (gamestarted)
        {
            gamestarted = false;
            inmainmenu = true;
        }
    }

    if (IsKeyReleased(KEY_ENTER))
    {
        if (curselected == 1)
        {
            SetWindowSize(1000, 700);
            ToggleFullscreen();

            if (selectedScreen == 0)
            {
                selectedScreen = 1;
            }
            else
            {
                selectedScreen = 0;
            }
        }
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        if (inmainmenu)
        {
            if (curselected == 0)
                inmainmenu = false;

            if (curselected == 2)
                CloseWindow();
        }
        else
        {
            if (curselected == 0) {
                mode = "oneball";
                gamestarted = true;
                InitGame();
            }
            if (curselected == 1) {
                mode = "multiball";
                gamestarted = true;
                InitGame();
            }
        }
    }
}

void DrawMenu(void)
{
    BeginDrawing();

    ClearBackground(BLACK);

    if (inmainmenu)
    {
        DrawText("Ccpong", GetScreenWidth() / 2 - MeasureText("Ccpong", 150) / 2, 100, 150, GRAY);

        DrawText("Play", GetScreenWidth() / 2 - MeasureText("Play", 40) / 2, 300, 40, textColorMENU1);

        DrawText(screenmodes[selectedScreen].c_str(), GetScreenWidth() / 2 - MeasureText(screenmodes[selectedScreen].c_str(), 40) / 2, 400, 40, textColorMENU2);

        DrawText("Exit", GetScreenWidth() / 2 - MeasureText("Exit", 40) / 2, 500, 40, textColorMENU3);

        DrawText("By Juan, Controls: W and S to scroll in the menus, BACKSPACE to back", GetScreenWidth() / 2 - MeasureText("By Juan, Controls: W and S to scroll in the menus, BACKSPACE to back", 10) / 2, 650, 10, GRAY);
    }
    else
    {
        DrawText("One ball", GetScreenWidth() / 2 - MeasureText("One ball", 40) / 2, 280, 40, textColor1);
        DrawText("Multi ball", GetScreenWidth() / 2 - MeasureText("Multi ball", 40) / 2, 400, 40, textColor2);
    }

    EndDrawing();
}

void UpdateMenuDrawFrame(void)
{
    UpdateMenu();
    DrawMenu();
}
