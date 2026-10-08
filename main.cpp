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

    const int WL = 10;
    const int HL = 10;
    Integer3DSpace<WL, HL> sworld{};
    sworld.createFlatFloor(0, DIRT);

    for (int i = 1; i < 3; i++)
    {
        sworld.createFlatFloor(i, SAND);
    }

    sworld.putBlockAt(DIRT, 5, 5, 5);
    sworld.putBlockAt(SAND, 5, 7, 5);
    sworld.putBlockAt(SAND, 5, 8, 5);
    sworld.putBlockAt(SAND, 5, 9, 5);

    SetTargetFPS(144); 
    
    int counter = 0;
    bool isAutomatonRun = false;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update

        if (isAutomatonRun)
            sworld.step();

        camera.moveCamera(GetFPS());

        if (IsKeyDown(KEY_SPACE))
        {
            isAutomatonRun = true;
        }
        
        if (IsKeyDown(KEY_P))
        {
            isAutomatonRun = false;
        }

        if (IsKeyDown(KEY_H))
        {
            printStackPartitions(sworld.getHeightMap(), HL, WL);
        }

        BeginDrawing();

            ClearBackground(RAYWHITE);
            BeginMode3D(camera);

            if(isAutomatonRun)
            {
                sworld.createOneBlockOnTop();
                
                if (counter%60 == 0)
                {
                }
                counter = (counter%60)+1;
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

            const string s1 = "| x=" + to_string((int) camera.position.x);
            const string s2 = "| y=" + to_string((int) camera.position.y);
            const string s3 = "| z=" + to_string((int) camera.position.z);

            DrawText("Show Heightmap [H]", 10, 80, 20, BLUE);
            DrawText("Lateral moves [Arrow{Up,Down,Left,Right}]", 10, 100, 20, BLUE);
            DrawText("Vertical moves [Numpad{8,2}]", 10, 120, 20, BLUE);
            DrawText(s1.c_str(), 10, 140, 20, BLACK);
            DrawText(s2.c_str(), 10, 160, 20, BLACK);
            DrawText(s3.c_str(), 10, 180, 20, BLACK);
            DrawFPS(10, 10);

        EndDrawing();

        counter = (counter+1)%60; 
    }

    CloseWindow(); 

    return 0;
}