#pragma once

#include "include/raylib.h"
#include "blocks.hpp"
// #include <array>
#include <vector>
#include <iostream>

using namespace std;

template<int WLEN, int HLEN>
class Integer3DSpace
{
private:
    vector<vector<vector<BlockType>>> mtx;
public:
    Integer3DSpace() : mtx(WLEN, vector<vector<BlockType>>(HLEN, vector<BlockType>(WLEN, AIR))) {}

    BlockType block(int x, int y, int z) const
    {
        if(x < 0 || WLEN <= x || y < 0 || HLEN <= y || z < 0 || WLEN <= z) return BORDER;
        return this->mtx[x][y][z];
    }

    BlockType & block(int x, int y, int z)
    {
        return this->mtx[x][y][z];
    }

    void createOneBlockOnTop()
    {
        this->block(rand()%WLEN, HLEN-1, rand()%WLEN) = SAND;
    }

    void step()
    {
        // Le sable tombe.

        for (int y = 0; y < HLEN; y++)
        {
            for (int x = 0; x < WLEN; x++)
            {
                for (int z = 0; z < WLEN; z++)
                {
                    BlockType & current_block = this->block(x, y, z);

                    switch(current_block)
                    {
                        case SAND:
                            if (this->block(x, y-1, z) == AIR)
                            {
                                this->block(x, y-1, z) = SAND;
                                this->block(x, y  , z) = AIR;
                            }

                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
    
    int volume()        const { return WLEN*HLEN*WLEN; }

    int getWidthSize()  const { return WLEN; }

    int getHeightSize() const { return HLEN; }

    int getDepthSize()  const { return WLEN; }
    
    void createFlatFloor(int y, BlockType block)
    {
        if (y < 0 || HLEN <= y)
        {
            cout << "y is out of bound." << endl;
            return;
        }

        for (int x = 0; x < WLEN; x++)
        {
            for (int z = 0; z < WLEN; z++)
            {  
                this->block(x, y, z) = block;
            }
        }
    }

    void drawWorld() const
    {
        for (int x = 0; x < WLEN; x++)
        {
            for (int y = 0; y < HLEN; y++)
            {
                for (int z = 0; z < WLEN; z++)
                {
                    Vector3 position{(float) x,(float) y,(float) z};

                    switch (this->block(x, y, z))
                    {
                        case DIRT:
                            DrawCube(position, 1.0f, 1.0f, 1.0f, BROWN);
                            break;
                        case SAND:
                            DrawCube(position, 1.0f, 1.0f, 1.0f, YELLOW);
                            DrawCubeWires(position, 1.0f, 1.0f, 1.0f, BROWN);
                            break;
                        case DEBUG:
                            DrawCube(position, 1.0f, 1.0f, 1.0f, PURPLE);
                            DrawCubeWires(position, 1.0f, 1.0f, 1.0f, ORANGE);
                            break;
                        case AIR:
                            break;
                    }
                }
            }
        }
    }
    
    void printWorld() const
    {
        for (int x = 0; x < WLEN; x++)
        {
            for (int y = 0; y < HLEN; y++)
            {
                for (int z = 0; z < WLEN; z++)
                {
                    cout << this->block(x, y, z);
                }
                cout << endl;
            }
            cout << endl;
        }
    }
};
