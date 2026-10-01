#include "include/raylib.h"

#include "blocks.hpp"
#include "i3dspace.hpp"

using namespace std;

int main(void)
{
    const int screenWidth = 800*2;
    const int screenHeight = 450*2;

    InitWindow(screenWidth, screenHeight, "Sand3D Prototype");

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, -10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f }; 
    camera.up = (Vector3){ 0.0f, 10.0f, 0.0f };
    camera.fovy = 45.0f; 
    camera.projection = CAMERA_PERSPECTIVE;

    const int WL = 100;
    const int HL = 100;
    Integer3DSpace<WL, HL> sworld{};
    sworld.createFlatFloor(0, DIRT);

    for (int i = 1; i < 3; i++)
    {
        sworld.createFlatFloor(i, SAND);
    }

    SetTargetFPS(30); 
    
    bool isAutomatonRun = false;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        if (isAutomatonRun)
            sworld.step();

        if (IsKeyDown(KEY_SPACE))
        {
            isAutomatonRun = true;
        }
        
        if (IsKeyDown(KEY_P))
        {
            isAutomatonRun = false;
        }

        if (IsKeyDown(KEY_RIGHT))
        {
            camera.position.x--;
            camera.target.x--;
        }

        if (IsKeyDown(KEY_LEFT))
        {
            camera.position.x++;
            camera.target.x++;
        }

        if (IsKeyDown(KEY_UP))
        {
            camera.position.z++;
            camera.target.z++;
        }

        if (IsKeyDown(KEY_DOWN))
        {
            camera.position.z--;
            camera.target.z--;
        }

        if (IsKeyDown(KEY_KP_8))
        {
            camera.position.y++;
            camera.target.y++;
        }

        if (IsKeyDown(KEY_KP_2))
        {
            camera.position.y--;
            camera.target.y--;
        }

        BeginDrawing();

            ClearBackground(RAYWHITE);
            BeginMode3D(camera);

            if(isAutomatonRun)
            {
                sworld.createOneBlockOnTop();
            }

            sworld.drawWorld();
            DrawGrid(20, 1.0f);

            EndMode3D();
            DrawText("SAND 3D SIMULATION", 10, 40, 20, DARKGRAY);

            if(isAutomatonRun)
            {
                DrawText("PRESS [P] TO PAUSE", 10, 60, 20, GREEN);
            } else {
                DrawText("PRESS [ESPACE] TO START", 10, 60, 20, RED);
            }
            
            DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow(); 

    return 0;
}