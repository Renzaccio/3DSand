#include "include/raylib.h"
#include "blocks.hpp"
#include "i3dspace.hpp"
#include "camera.hpp"
#include <string>

using namespace std;

int main(void)
{
    const int screenWidth = 800*2;
    const int screenHeight = 450*2;

    InitWindow(screenWidth, screenHeight, "Sand3D Prototype");

    OrientatedCamera camera{};

    const int WL = 40;
    const int HL = 40;
    Int3DSpace<WL, HL> sworld{};
    sworld.createFlatFloor(0, DIRT);

    // for (int i = 1; i < 3; i++) sworld.createFlatFloor(i, SAND);

    sworld.block(5, 5, 5) = DIRT;
    sworld.block(5, 7, 5) = SAND;
    sworld.block(5, 8, 5) = SAND;
    sworld.block(5, 9, 5) = SAND;

    int targetFPS = 144;
    SetTargetFPS(targetFPS);

    int simulationCounter = 0;
    int simulationPerSecond = 40;
    int spawnCounter = 0;
    int spawnPerSecond = 40;
    int clock = 0;

    bool isAutomatonRun = false;

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update

        camera.moveCamera(GetFPS());

        if (IsKeyPressed(KEY_SPACE))
            isAutomatonRun = !isAutomatonRun;

        if(isAutomatonRun)
        {
            if (simulationCounter <= 0)
            {
                sworld.step();

                if (IsKeyDown(KEY_L))
                {
                    if (spawnCounter <= 0)
                    {
                        sworld.createOneBlockOnTop();
                        spawnCounter = simulationPerSecond / spawnPerSecond;
                    }

                    spawnCounter--;
                }
                else
                {
                    spawnCounter = 0;
                }

                simulationCounter = GetFPS() / simulationPerSecond;
            }

            simulationCounter--;
        }
        else
        {
            simulationCounter = 0;
            spawnCounter = 0;
        }

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                sworld.drawWorld();
                // DrawGrid(20, 1.0f);

            EndMode3D();


            DrawFPS(10, 10);
            const string s0 = to_string(spawnCounter);
            DrawText(s0.c_str(), 150, 10, 20, DARKGRAY);
            DrawText("SAND 3D SIMULATION", 10, 40, 20, DARKGRAY);

            if(isAutomatonRun)
                DrawText("PRESS [SPACE] TO PAUSE", 10, 60, 20, GREEN);
            else
                DrawText("PRESS [SPACE] TO START", 10, 60, 20, RED);

            DrawText("Print Heightmap [ H ]", 10, 80, 20, BLUE);
            DrawText("Lateral Moves [ ZQSD | WASD ]", 10, 100, 20, BLUE);
            DrawText("Vertical Moves [ E/A | E/Q ]", 10, 120, 20, BLUE);
            DrawText("Rotating Camera [ Arrow Keys ]", 10, 140, 20, BLUE);
            DrawText("Fast/Slow Moves [ Left Shift / Left Ctrl ]", 10, 160, 20, BLUE);

            const string s1 = "| x=" + to_string((int) camera.position.x);
            const string s2 = "| y=" + to_string((int) camera.position.y);
            const string s3 = "| z=" + to_string((int) camera.position.z);
            DrawText(s1.c_str(), 10, 180, 20, BLACK);
            DrawText(s2.c_str(), 10, 200, 20, BLACK);
            DrawText(s3.c_str(), 10, 220, 20, BLACK);

        EndDrawing();

        clock++;
    }

    CloseWindow(); 

    return 0;
}