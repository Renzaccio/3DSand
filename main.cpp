#include <string>
#include "counter.cpp"
#include "camera.hpp"
#include "i3dspace.hpp"
#include "include/raylib.h"
#include "ui.hpp"

using namespace std;

int main(void)
{
    const static int screenWidth = 1600;
    const static int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Sand3D Prototype");

    OrientatedCamera camera{};

    const static int WL = 75;
    const static int HL = 30;
    const static int DL = 75;
    Int3DSpace<WL, HL, DL> sworld{};
    sworld.createFlatFloor(0, STONE);
    sworld.createFlatFloor(1, DIRT);

    for (int i = 2; i < 4; i++)
        sworld.createFlatFloor(i, SAND);

    sworld.updateWorldVoxelVisibility();
    
    // placeChair3x4x3(sworld.getWorld(), {2,2,2});
    // placeTable5x5x5(sworld.getWorld(), {10,2,10});

    // sworld.block(5, 7, 5) = SAND;
    // sworld.block(5, 8, 5) = SAND;
    // sworld.block(5, 9, 5) = SAND;

    int targetFPS = GetMonitorRefreshRate(GetCurrentMonitor());

    int simulationCounter = 0;
    const static int simulationPerSecond = 30;
    int spawnCounter = 0;
    const static int spawnPerSecond = 30;
    int clock = 0;

    int sandAmount = 0;
    int staticBlockAmount = 0;

    bool isAutomatonRun = false;
    bool isSpawningRun = false;
    {
        array<int, 2> simuBlockStat = sworld.countBlockStats();
        sandAmount = simuBlockStat[0];
        staticBlockAmount = simuBlockStat[1];
    }

    const static int fontSize = 20;

    vector<RPressButton> rbtns{ // Buttons for changing FPS
        RPressButton {10,                10, fontSize * 2, fontSize * 9 / 10, RED,   [&targetFPS]() { if (targetFPS > 15) targetFPS -= 10; else targetFPS = 5; }, " -10 ", fontSize, BLACK},
        RPressButton {20 + fontSize * 2, 10, fontSize * 2, fontSize * 9 / 10, RED,   [&targetFPS]() { if (targetFPS >  5) targetFPS--;                         }, " -1 ",  fontSize, BLACK},
        RPressButton {30 + fontSize * 4, 10, fontSize * 2, fontSize * 9 / 10, GREEN, [&targetFPS]() {                     targetFPS++;                         }, " +1 ",  fontSize, BLACK},
        RPressButton {40 + fontSize * 6, 10, fontSize * 2, fontSize * 9 / 10, GREEN, [&targetFPS]() {                     targetFPS += 10;                     }, " +10 ", fontSize, BLACK},
    };

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        SetTargetFPS(targetFPS);
        camera.moveCamera(GetFPS());

        if (IsKeyPressed(KEY_SPACE))
            isAutomatonRun = !isAutomatonRun;

        if (IsKeyPressed(KEY_R))
            isSpawningRun = !isSpawningRun;

        if(isAutomatonRun)
        {
            if (simulationCounter <= 0)
            {
                sworld.step();

                if (isSpawningRun)
                {
                    if (spawnCounter <= 0)
                    {
                        int flow = 2;

                        sworld.createOneBlockOnTop(
                            (sworld.getWidth() - flow)/2,
                            sworld.getWidth()/2 + flow,
                            (sworld.getDepth() - flow)/2,
                            sworld.getDepth()/2 + flow
                        );

                        // sworld.createOneBlockOnTop(
                        //     (sworld.getWidth() - flow)/2 + 10,
                        //     sworld.getWidth()/2 + flow + 10,
                        //     (sworld.getDepth() - flow)/2,
                        //     sworld.getDepth()/2 + flow
                        // );

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

            if ((clock % targetFPS) == 0) // Run once every seconds
            {
                // Counting Blocks
                array<int, 2> simuBlockStat = sworld.countBlockStats();
                sandAmount = simuBlockStat[0];
                staticBlockAmount = simuBlockStat[1];

                // Updating Blocks visibility
                sworld.updateWorldVoxelVisibility();
            }
        }
        else
        {
            simulationCounter = 0;
            spawnCounter = 0;
        }

        // check if btns is pressed
        for (RPressButton rpb : rbtns)
            rpb.executeIfPressed();

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
            {
                sworld.drawWorld();
                // DrawGrid(20, 1.0f);
            }
            EndMode3D();


            for (RPressButton rpb : rbtns)
                rpb.draw();

            Counter<fontSize> posY(10);

            DrawText((to_string(targetFPS) + " FPS MAX").c_str(), 50 + fontSize * 8, posY(), fontSize, BLACK);
            DrawText((to_string(GetFPS()) + " FPS").c_str(), 10, posY(), fontSize, DARKGREEN);

            const string s01 = to_string(sandAmount) + " sand" + ((sandAmount > 1) ? "s" : "") + " / " + to_string(staticBlockAmount) + " static block" + ((staticBlockAmount > 1) ? "s" : "");
            DrawText(s01.c_str(), 10, posY(), fontSize, DARKGRAY);

            posY.step();

            DrawText("SAND 3D SIMULATION", 10, posY(), fontSize, DARKGRAY);

            if(isAutomatonRun) DrawText("PRESS [ SPACE ] TO PAUSE", 10, posY(), fontSize, GREEN);
            else               DrawText("PRESS [ SPACE ] TO START", 10, posY(), fontSize, RED);

            if(isSpawningRun) DrawText("PRESS [ R ] TO DISABLE SAND POURING", 10, posY(), fontSize, GREEN);
            else              DrawText("PRESS [ R ] TO ENABLE SAND POURING", 10, posY(), fontSize, RED);

            DrawText("Lateral Moves [ ZQSD | WASD ]", 10, posY(), fontSize, BLUE);
            DrawText("Vertical Moves [ E/A | E/Q ]", 10, posY(), fontSize, BLUE);
            DrawText("Rotating Camera [ Arrow Keys ]", 10, posY(), fontSize, BLUE);
            DrawText("Fast/Slow Moves [ Left Shift / Left Ctrl ]", 10, posY(), fontSize, BLUE);

            posY.step();

            const string s1 = "| x=" + to_string((int) camera.position.x);
            const string s2 = "| y=" + to_string((int) camera.position.y);
            const string s3 = "| z=" + to_string((int) camera.position.z);
            DrawText(s1.c_str(), 10, posY(), fontSize, BLACK);
            DrawText(s2.c_str(), 10, posY(), fontSize, BLACK);
            DrawText(s3.c_str(), 10, posY(), fontSize, BLACK);
        }
        EndDrawing();

        clock++;
    }

    CloseWindow(); 

    return 0;
}