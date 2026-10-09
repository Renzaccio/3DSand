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
        int randX = rand()%WLEN;
        int randY = rand()%WLEN;
        this->mtx[randX][HLEN-1][randY] = SAND;
    }

    void step()
    {
        // Le sable tombe

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
    
    int volume()
    {
        return WLEN*WLEN*HLEN;
    }

    int getHeightSize() const
    {
        return HLEN;
    }

    int getWidthSize() const
    {
        return WLEN;
    }

    int getDepthSize() const
    {
        return WLEN;
    }
    
    void createFlatFloor(int y, BlockType block)
    {
        if (y >= HLEN)
        {
            cout << "y est trop grand..." << endl;
            return;
        }

        for (int i = 0; i < WLEN; i++)
        {
            for (int j = 0; j < WLEN; j++)
            {
                this->mtx[i][y][j] = block;
            }
        }
    }

    BlockType getVoxelAt(unsigned int x, unsigned int y, unsigned int z) const
    {
        return this->mtx[x][y][z];
    }

    void drawWorld() const
    {
        for (int i = 0; i < WLEN; i++)
        {
            for (int j = 0; j < HLEN; j++)
            {
                for (int k = 0; k < WLEN; k++)
                {
                    Vector3 position{(float) i,(float) j,(float) k};

                    switch (this->mtx[i][j][k])
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
    
    void printWorld()
    {
        for (int i = 0; i < WLEN; i++)
        {
            for (int j = 0; j < HLEN; j++)
            {
                for (int k = 0; k < WLEN; k++)
                {
                    cout << this->mtx[i][j][k]; 
                }
                cout << endl;
            }
            cout << endl;
        }
    }
};
