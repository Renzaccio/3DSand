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

    int targetFPS = GetMonitorRefreshRate(GetCurrentMonitor()); SetTargetFPS(targetFPS);

    OrientatedCamera camera{};


    //// WORLD SETUP ////
    const static int WL = 75;
    const static int HL = 50;
    const static int DL = 75;
    Int3DSpace<WL, HL, DL> sworld{};

    for (int i = 0; i < 4; i++)
        sworld.placeFlatFloor(i, STONE);
    sworld.placeFlatFloor(4, DIRT);

    sworld.addNewStaticStructure(createChair(Vec3I { 2, 5,  2 }));
    sworld.addNewStaticStructure(createTable(Vec3I { 2, 5, 6 }));

    sworld.placeParallelepiped({ 20,  0, 20 }, { 40, 20, 40 }, STONE);
    sworld.placeParallelepiped({ 22,  2, 22 }, { 38, 20, 38 }, AIR);
    sworld.placeParallelepiped({ 24,  17, 20 }, { 25, 18, 21 }, AIR);
    sworld.placeParallelepiped({ 22,  2, 22 }, { 38, 15, 38 }, WATER);

    sworld.updateWorldVoxelSurfaceVisibility();

    int staticBlocksAmount = 0;
    int movingBlocksAmount = 0;
    { // Conting initial blocks amounts
        pair<int, int> BlocksAmount{ 0, 0 };
        sworld.countBlockStats(BlocksAmount);
        staticBlocksAmount = BlocksAmount.first;
        movingBlocksAmount = BlocksAmount.second;
    }
    //// End of WORLD SETUP ////


    //// CLOCK SETUP ////
    int simulationCounter = 0;
    const static int simulationPerSecond = 30;
    bool isAutomatonRun = false;

    int spawnCounter = 0;
    const static int spawnPerSecond = 30;
    bool isSpawningRun = false;

    int clock = 0;
    //// End of CLOCK SETUP ////


    //// UI SETUP ////
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
    //// End of UI SETUP ////


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
                // for(int i = 0; i < 1; i++)
                    sworld.step();

                if (isSpawningRun)
                {
                    if (spawnCounter <= 0)
                    {
                        int flow = 5;

                        for(int i = 0; i < 10; i++)
                        {
                            sworld.placeOneBlockOnTop(
                                (sworld.getWidth() - flow)/2,
                                sworld.getWidth()/2 + flow,
                                (sworld.getDepth() - flow)/2,
                                sworld.getDepth()/2 + flow,
                                SAND
                            );

                            sworld.placeOneBlockOnTop(
                                (sworld.getWidth() - flow)/2,
                                sworld.getWidth()/2 + flow,
                                (sworld.getDepth() - flow)/2,
                                sworld.getDepth()/2 + flow,
                                WATER
                            );
                        }

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

            if ((clock % targetFPS) <= 0) // Run once every seconds
            {
                // Counting Blocks
                pair<int, int> BlocksAmount{ 0, 0 };
                sworld.countBlockStats(BlocksAmount);
                staticBlocksAmount = BlocksAmount.first;
                movingBlocksAmount = BlocksAmount.second;

                // Updating Blocks visibility
                sworld.updateWorldVoxelSurfaceVisibility();
            }
        }
        else
        {
            simulationCounter = 0;
            spawnCounter = 0;
        }

        // Button clicking
        fpsBtnGroup.pressDetection();
        menu.pressDetection();

        // Mouse interaction with the world
        menuRaySupport.rayTest<WL, HL, DL>(sworld, camera, menu);

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            //// 3D RENDER ////
            BeginMode3D(camera);
            {
                sworld.drawWorld();
                // DrawGrid(20, 1.0f);

                camera.drawInfo();
            }
            EndMode3D();

            if (sworld.getBlock(camera.getPos()) == WATER)
                DrawRectangle(0, 0, screenWidth, screenWidth, Color{ 0, 121, 241, 92 });
            //// End of 3D RENDER ////


            //// USER INTERFACE ////
            fpsBtnGroup.drawAll();
            menu.draw();

            Counter<fontSize> posY(borderWidth);

            DrawText((to_string(targetFPS) + " FPS MAX").c_str(), buttonX.get(), posY(), fontSize, BLACK);
            DrawText((to_string(GetFPS()) + " FPS").c_str(),      borderWidth,   posY(), fontSize, DARKGREEN);

            const string s01 = to_string(staticBlocksAmount) + " static block" + ((staticBlocksAmount > 1) ? "s" : "") + " / " + to_string(movingBlocksAmount) + " moving block" + ((movingBlocksAmount > 1) ? "s" : "");
            DrawText(s01.c_str(), borderWidth, posY(), fontSize, DARKGRAY);

            posY.step();

            DrawText("SAND 3D SIMULATION", borderWidth, posY(), fontSize, DARKGRAY);

            if (isAutomatonRun) DrawText("PRESS [ SPACE ] TO PAUSE", borderWidth, posY(), fontSize, GREEN);
            else                DrawText("PRESS [ SPACE ] TO START", borderWidth, posY(), fontSize, RED);

            if (isSpawningRun) DrawText("PRESS [ R ] TO DISABLE SAND POURING", borderWidth, posY(), fontSize, GREEN);
            else               DrawText("PRESS [ R ] TO ENABLE SAND POURING",  borderWidth, posY(), fontSize, RED);

            if (camera.getIsInFirstPerson()) DrawText("PRESS [ F ] TO ENABLE LAID BACK CAMERA",    borderWidth, posY(), fontSize, VIOLET);
            else                             DrawText("PRESS [ F ] TO ENABLE FIRST PERSON CAMERA", borderWidth, posY(), fontSize, VIOLET);

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
            //// End of USER INTERFACE ////
        }
        EndDrawing();

        clock++;
    }

    cout << endl;

    CloseWindow();

    return 0;
}