#include "app.h"

#include "defs.h"
#include "entity.h"
#include "raylib.h"


namespace rmp {
    constexpr int screenWidth = 1024;
    constexpr int screenHeight = 768;

    App::App() {
    }

    App::~App() {
    }

    int App::Run() {

        InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

        SetTargetFPS(60);              

        while (!WindowShouldClose())   
        {

            BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

            EndDrawing();

        }

        CloseWindow();       

        return 0;
    }
}