#pragma once

#include "include/raylib.h"
#include "blocks.hpp"
#include <array>
#include <vector>
#include <iostream>

using namespace std;

template<int WLEN, int HLEN>
class Integer3DSpace
{
private:
    vector<vector<vector<BlockType>>> mtx;
    const BlockType border = BORDER;
public:
    Integer3DSpace() : mtx(WLEN, vector<vector<BlockType>>(HLEN, vector<BlockType>(WLEN, AIR))) {}

    const BlockType & block2(int x, int y, int z) const
    {
        if(x < 0 || WLEN <= x || y < 0 || HLEN <= y || z < 0 || WLEN <= z) return this->border;
        return this->mtx[x][y][z];
    }

    BlockType & block(int x, int y, int z)
    {
        return this->mtx[x][y][z];
    }
    
    const BlockType & block2(const array<int, 3> & v3) const
    {
        return block2(v3[0], v3[1], v3[2]);
    }

    BlockType & block(array<int, 3> v3)
    {
        return block(v3[0], v3[1], v3[2]);
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
                    BlockType & currentBlock = this->block(x, y, z);

                    switch(currentBlock)
                    {
                        default:
                        {
                            break;
                        }
                        case SAND:
                        {
                            {
                                BlockType blockToTest = this->block(x, y-1, z);

                                if (blockToTest == AIR)
                                {
                                    this->block(x, y-1, z) = SAND;
                                    currentBlock = AIR;
                                    break;
                                }
                            }
                            
                            vector<array<int, 3>> blocksToTestCoords;

                            blocksToTestCoords.push_back({ x  , y-1, z-1 });
                            blocksToTestCoords.push_back({ x-1, y-1, z   });
                            blocksToTestCoords.push_back({ x  , y-1, z+1 });
                            blocksToTestCoords.push_back({ x+1, y-1, z   });
                            
                            for (auto blockToTestCoord : blocksToTestCoords)
                            {
                                const BlockType & blockToTest = this->block2(blockToTestCoord);

                                if (blockToTest == AIR)
                                {
                                    this->block(blockToTestCoord) = SAND;
                                    currentBlock = AIR;
                                    break;
                                }
                            }

                            break;
                        }
                    }
                }
            }
        }
    }
    
    int volume() const { return WLEN*HLEN*WLEN; }

    int getWidthSize() const { return WLEN; }

    int getHeightSize() const { return HLEN; }

    int getDepthSize() const { return WLEN; }
    
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
        float horizontalShift = - WLEN / 2.0f + 0.5f;

        for (int x = 0; x < WLEN; x++)
        {
            for (int y = 0; y < HLEN; y++)
            {
                for (int z = 0; z < WLEN; z++)
                {
                    Vector3 position{x + horizontalShift,y + 0.5f,z + horizontalShift};

                    switch (this->block2(x, y, z))
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
                    cout << this->block2(x, y, z);
                }
                cout << endl;
            }
            cout << endl;
        }
    }
};
