#include <string>
#include "include/ui.hpp"
#include "include/camera.hpp"

using namespace std;

int main(void)
{
    const static int screenWidth = 1600;
    const static int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Sand3D Prototype");
    SetWindowMonitor(GetMonitorCount() - 1);
    SetWindowFocused();

    cout << endl;

    OrientatedCamera camera{};

    const static int WL = 75;
    const static int HL = 30;
    const static int DL = 75;
    Int3DSpace<WL, HL, DL> sworld{};
    for (int i = 0; i < 4; i++)
        sworld.placeFlatFloor(i, STONE);
    sworld.placeFlatFloor(4, DIRT);

    for (int i = 10; i < 12; i++)
        sworld.placeFlatFloor(i, SAND);

    sworld.placeChair3x4x3({  2, 5,  2 });
    sworld.placeTable5x4x5({ 10, 5, 10 });

    // OOB place() calls for testing
    // sworld.placeFlatFloor(100, DIRT);
    // sworld.placeChair3x4x3({  9, 2, -1 });
    // sworld.placeTable5x4x5({ -3, 2,  8 });

    sworld.updateWorldVoxelSurfaceVisibility();

    int targetFPS = GetMonitorRefreshRate(GetCurrentMonitor()); SetTargetFPS(targetFPS);

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
        array<int, 2> blockAmountArr{ 0, 0 };
        sworld.countBlockStats(blockAmountArr);
        sandAmount = blockAmountArr[0];
        staticBlockAmount = blockAmountArr[1];
    }

    const static int fontSize = 20;
    const static int borderWidth = 10;

    Counter<fontSize * 3> buttonX(borderWidth);

    RPressButtonLogicalGroup fpsBtnGroup{{ // Buttons for changing FPS
        RPressButton { (float)buttonX(), borderWidth, fontSize * 8 / 3, fontSize * 9 / 10, RED,       [&targetFPS](RPressButton& r) { if (targetFPS > 15) targetFPS -= 10; else targetFPS = 5;  SetTargetFPS(targetFPS); }, "-10", fontSize, BLACK },
        RPressButton { (float)buttonX(), borderWidth, fontSize * 8 / 3, fontSize * 9 / 10, RED,       [&targetFPS](RPressButton& r) { if (targetFPS >  5) targetFPS--;                          SetTargetFPS(targetFPS); }, "-1",  fontSize, BLACK },
        RPressButton { (float)buttonX(), borderWidth, fontSize * 8 / 3, fontSize * 9 / 10, LIGHTGRAY, [&targetFPS](RPressButton& r) { targetFPS = GetMonitorRefreshRate(GetCurrentMonitor());   SetTargetFPS(targetFPS); }, "RES", fontSize,  BLACK },
        RPressButton { (float)buttonX(), borderWidth, fontSize * 8 / 3, fontSize * 9 / 10, GREEN,     [&targetFPS](RPressButton& r) { targetFPS++;                                              SetTargetFPS(targetFPS); }, "+1",  fontSize, BLACK },
        RPressButton { (float)buttonX(), borderWidth, fontSize * 8 / 3, fontSize * 9 / 10, GREEN,     [&targetFPS](RPressButton& r) { targetFPS += 10;                                          SetTargetFPS(targetFPS); }, "+10", fontSize, BLACK },
    }};

    DockRay menuRaySupport;
    Dock menu = buildMenu(menuRaySupport);

    cout << endl;

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        camera.moveCamera(GetFPS());

        if (IsKeyPressed(KEY_SPACE))
            isAutomatonRun = !isAutomatonRun;

        if (IsKeyPressed(KEY_R))
            isSpawningRun = !isSpawningRun;

        if (isAutomatonRun)
        {
            if (simulationCounter <= 0)
            {
                sworld.step();

                if (isSpawningRun)
                {
                    if (spawnCounter <= 0)
                    {
                        int flow = 2;

                        sworld.placeOneBlockOnTop(
                            (sworld.getWidth() - flow)/2,
                            sworld.getWidth()/2 + flow,
                            (sworld.getDepth() - flow)/2,
                            sworld.getDepth()/2 + flow
                        );

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
                array<int, 2> blockAmountArr{ 0, 0 };
                sworld.countBlockStats(blockAmountArr);
                sandAmount = blockAmountArr[0];
                staticBlockAmount = blockAmountArr[1];

                // Updating Blocks visibility
                sworld.updateWorldVoxelSurfaceVisibility();
            }
        }
        else
        {
            simulationCounter = 0;
            spawnCounter = 0;
        }

        fpsBtnGroup.pressDetection();
        menu.pressDetection();
        menuRaySupport.rayTest<WL, HL, DL>(sworld, camera, menu);

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
            {
                sworld.drawWorld();
                // DrawGrid(20, 1.0f);

                camera.drawInfo();
            }
            EndMode3D();

            fpsBtnGroup.drawAll();
            menu.draw();

            Counter<fontSize> posY(borderWidth);

            DrawText((to_string(targetFPS) + " FPS MAX").c_str(), buttonX.get(), posY(), fontSize, BLACK);
            DrawText((to_string(GetFPS()) + " FPS").c_str(),      borderWidth,   posY(), fontSize, DARKGREEN);

            const string s01 = to_string(sandAmount) + " sand" + ((sandAmount > 1) ? "s" : "") + " / " + to_string(staticBlockAmount) + " static block" + ((staticBlockAmount > 1) ? "s" : "");
            DrawText(s01.c_str(), borderWidth, posY(), fontSize, DARKGRAY);

            posY.step();

            DrawText("SAND 3D SIMULATION", borderWidth, posY(), fontSize, DARKGRAY);

            if (isAutomatonRun) DrawText("PRESS [ SPACE ] TO PAUSE", borderWidth, posY(), fontSize, GREEN);
            else               DrawText("PRESS [ SPACE ] TO START", borderWidth, posY(), fontSize, RED);

            if (isSpawningRun) DrawText("PRESS [ R ] TO DISABLE SAND POURING", borderWidth, posY(), fontSize, GREEN);
            else              DrawText("PRESS [ R ] TO ENABLE SAND POURING", borderWidth, posY(), fontSize, RED);

            if (camera.getIsInFirstPerson()) DrawText("PRESS [ F ] TO ENABLE LAID BACK CAMERA", borderWidth, posY(), fontSize, ORANGE);
            else                             DrawText("PRESS [ F ] TO ENABLE FIRST PERSON CAMERA", borderWidth, posY(), fontSize, ORANGE);

            DrawText("Lateral Moves [ ZQSD | WASD ]",              borderWidth, posY(), fontSize, BLUE);
            DrawText("Vertical Moves [ E/A | E/Q ]",               borderWidth, posY(), fontSize, BLUE);
            DrawText("Rotating Camera [ Arrow Keys ]",             borderWidth, posY(), fontSize, BLUE);
            DrawText("Fast/Slow Moves [ Left Shift / Left Ctrl ]", borderWidth, posY(), fontSize, BLUE);

            posY.step();

            const string s1 = "| x=" + to_string((int) camera.position.x);
            const string s2 = "| y=" + to_string((int) camera.position.y);
            const string s3 = "| z=" + to_string((int) camera.position.z);
            DrawText(s1.c_str(), borderWidth, posY(), fontSize, BLACK);
            DrawText(s2.c_str(), borderWidth, posY(), fontSize, BLACK);
            DrawText(s3.c_str(), borderWidth, posY(), fontSize, BLACK);
        }
        EndDrawing();

        clock++;
    }

    cout << endl;

    CloseWindow();

    return 0;
}