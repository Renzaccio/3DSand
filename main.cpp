#include "include/raylib.h"
// #include "raylib.h"
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

enum BlockType { AIR, SAND, DIRT };

template<int SLEN>
class Integer3DSpace
{
private:
    vector<vector<vector<BlockType>>> mtx;
public: 
    Integer3DSpace() : mtx(SLEN, vector<vector<BlockType>>(SLEN, vector<BlockType>(SLEN, AIR))) {}
    
    int area()
    {
        return SLEN*SLEN;
    }

    int size_of_length()
    {
        return SLEN;
    }
    
    void createFlatFloor(int y, BlockType block)
    {
        for (int i = 0; i < SLEN; i++)
        {
            for (int j = 0; j < SLEN; j++)
            {
                this->mtx[i][y][j] = block;
            }
        }
    }

    BlockType getVoxelAt(unsigned int x, unsigned int y, unsigned int z)
    {
        return this->mtx[x][y][z];
    }

    void updateAutomaton()
    {

    }

    void drawWorld()
    {
        for (int i = 0; i < SLEN; i++)
        {
            for (int j = 0; j < SLEN; j++)
            {
                for (int k = 0; k < SLEN; k++)
                {
                    Vector3 position{i,j,k};
                    // DrawCube(cubePosition, 1.0f, 1.0f, 1.0f, RED);
                    // DrawCubeWires(cubePosition, 1.0f, 1.0f, 1.0f, BLACK);

                    switch (this->mtx[i][j][k])
                    {
                        case DIRT:
                            DrawCube(position, 1.0f, 1.0f, 1.0f, BROWN);
                            break;
                        case AIR:
                            break;
                    }
                }
            }
        }
    }
    
    void printWorld()
    {
        for (int i = 0; i < SLEN; i++)
        {
            for (int j = 0; j < SLEN; j++)
            {
                for (int k = 0; k < SLEN; k++)
                {
                    cout << this->mtx[i][j][k]; 
                }
                cout << endl;
            }
            cout << endl;
        }
    }
};

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Sand3D Prototype");

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, -10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f }; 
    camera.up = (Vector3){ 0.0f, 10.0f, 0.0f };
    camera.fovy = 45.0f; 
    camera.projection = CAMERA_PERSPECTIVE;

    Integer3DSpace<40> sworld{};
    sworld.createFlatFloor(0, DIRT);

    SetTargetFPS(60); 

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
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

            sworld.drawWorld();
            DrawGrid(20, 1.0f);

            EndMode3D();
            DrawText("SAND 3D SIMULATION", 10, 40, 20, DARKGRAY);
            DrawText("PRESS [ESPACE] TO START", 10, 60, 20, DARKGRAY);
            DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow(); 

    return 0;
}