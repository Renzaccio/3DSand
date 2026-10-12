#include <string>
#include "counter.cpp"
#include "camera.hpp"
#include "i3dspace.hpp"
#include "ui.hpp"

using namespace std;

int main(void)
{
    const int screenWidth = 800*2;
    const int screenHeight = 450*2;

    InitWindow(screenWidth, screenHeight, "Sand3D Prototype");

    OrientatedCamera camera{};

    const int WL = 100;
    const int HL = 70;
    const int DL = 100;
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

    int targetFPS = 60;

    int simulationCounter = 0;
    int simulationPerSecond = 30;
    int spawnCounter = 0;
    int spawnPerSecond = 30;
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

    vector<RPressButton> rbtns{
        // Change fps max, +1 or -1
        RPressButton {10.0f, 10.0f, 26.0f, 19.0f, RED,   [&targetFPS]() { if (targetFPS > 5) targetFPS--; cout << targetFPS << endl; }, " - ", 20, BLACK},
        RPressButton {46.0f, 10.0f, 26.0f, 19.0f, GREEN, [&targetFPS]() {                    targetFPS++; cout << targetFPS << endl; }, " + ", 20, BLACK},
        //
    };

    int

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        cout << targetFPS << endl;
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

            if ((clock % targetFPS) == 0) // blocks are counted only once every second
            {
                array<int, 2> simuBlockStat = sworld.countBlockStats();
                sandAmount = simuBlockStat[0];
                staticBlockAmount = simuBlockStat[1];
            }

            if ((clock % (5*targetFPS)) == 0) // blocks are counted only once every 5 seconds - florent propose better optimisation by neighbors approche
            {
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
            if (CheckCollisionPointRec(GetMousePosition(), rpb))
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    rpb();

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

            Counter<20> posY(10);

            DrawText((to_string(targetFPS) + " FPS MAX").c_str(), 82, posY(), 20, BLACK);
            DrawFPS(10, posY());

            const string s01 = to_string(sandAmount) + " sand" + ((sandAmount > 1) ? "s" : "") + " / " + to_string(staticBlockAmount) + " static block" + ((staticBlockAmount > 1) ? "s" : "");
            DrawText(s01.c_str(), 10, posY(), 20, DARKGRAY);

            posY.step();

            DrawText("SAND 3D SIMULATION", 10, posY(), 20, DARKGRAY);

            if(isAutomatonRun) DrawText("PRESS [ SPACE ] TO PAUSE", 10, posY(), 20, GREEN);
            else               DrawText("PRESS [ SPACE ] TO START", 10, posY(), 20, RED);

            if(isSpawningRun) DrawText("PRESS [ R ] TO DISABLE SAND POURING", 10, posY(), 20, GREEN);
            else              DrawText("PRESS [ R ] TO ENABLE SAND POURING", 10, posY(), 20, RED);

            DrawText("Lateral Moves [ ZQSD | WASD ]", 10, posY(), 20, BLUE);
            DrawText("Vertical Moves [ E/A | E/Q ]", 10, posY(), 20, BLUE);
            DrawText("Rotating Camera [ Arrow Keys ]", 10, posY(), 20, BLUE);
            DrawText("Fast/Slow Moves [ Left Shift / Left Ctrl ]", 10, posY(), 20, BLUE);

            posY.step();

            const string s1 = "| x=" + to_string((int) camera.position.x);
            const string s2 = "| y=" + to_string((int) camera.position.y);
            const string s3 = "| z=" + to_string((int) camera.position.z);
            DrawText(s1.c_str(), 10, posY(), 20, BLACK);
            DrawText(s2.c_str(), 10, posY(), 20, BLACK);
            DrawText(s3.c_str(), 10, posY(), 20, BLACK);
        }
        EndDrawing();

        clock++;
    }

    CloseWindow(); 

    return 0;
}