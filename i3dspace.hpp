#pragma once

#include "include/raylib.h"
#include "include/raymath.h"
#include "blocks.hpp"
#include <array>
#include <vector>
#include <iostream>

using namespace std;

template<int WLEN, int HLEN, int DLEN>
class Int3DSpace
{
private:
    vector<vector<vector<BlockType>>> mtx;
    const BlockType border = BORDER;

    Vector3 drawShift = {
        0.5f - WLEN / 2.0f,
        0.5f,
        0.5f - WLEN / 2.0f
    };
public:
    Int3DSpace() : mtx(WLEN, vector<vector<BlockType>>(HLEN, vector<BlockType>(DLEN, AIR))) {}



    BlockType & block(int x, int y, int z)
    {
        return this->mtx[x][y][z];
    }

    BlockType & block(const array<int, 3> & v3)
    {
        return this->block(v3[0], v3[1], v3[2]);
    }

    const BlockType & block(int x, int y, int z) const
    {
        if(x < 0 || WLEN <= x || y < 0 || HLEN <= y || z < 0 || DLEN <= z) return this->border;
        return this->mtx[x][y][z];
    }

    const BlockType & blockConst(int x, int y, int z) const
    {
        return this->block(x, y, z);
    }
    
    const BlockType & block(const array<int, 3> & v3) const
    {
        return this->block(v3[0], v3[1], v3[2]);
    }
    
    const BlockType & blockConst(const array<int, 3> & v3) const
    {
        return this->block(v3);
    }



    void createOneBlockOnTop(int xStart = 0, int xEnd = WLEN, int zStart = 0, int zEnd = WLEN)
    {
        this->block(rand() % (xStart - xEnd) + xStart, HLEN - 1, rand() % (zStart - zEnd) + zStart) = SAND;
    }

    void step()
    {
        // Le sable tombe.

        for (int y = 0; y < HLEN; y++)
            for (int x = 0; x < WLEN; x++)
                for (int z = 0; z < DLEN; z++)
                    this->voxelStep(x, y, z);
    }
    
    void voxelStep(int x, int y, int z)
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
                    const BlockType & blockToTest = this->blockConst(x, y-1, z);

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
                    const BlockType & blockToTest = this->blockConst(blockToTestCoord);

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
    
    int getVolume() const { return WLEN*HLEN*DLEN; }

    int getWidth() const { return WLEN; }

    int getHeight() const { return HLEN; }

    int getDepth() const { return DLEN; }
    
    void createFlatFloor(int y, BlockType block)
    {
        if (y < 0 || HLEN <= y)
        {
            cout << "y is out of bound." << endl;
            return;
        }

        for (int x = 0; x < WLEN; x++)
            for (int z = 0; z < DLEN; z++)
                this->block(x, y, z) = block;
    }

    void drawWorld() const
    {
        for (int x = 0; x < WLEN; x++)
            for (int y = 0; y < HLEN; y++)
                for (int z = 0; z < DLEN; z++)
                    this->drawVoxel(x, y, z);
    }

    void drawVoxel(int x, int y, int z) const
    {
        Vector3 voxelPosition = { (float) x, (float) y, (float) z };
        Vector3 position = voxelPosition + this->drawShift;

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
    
    void printWorld() const
    {
        for (int x = 0; x < WLEN; x++)
        {
            for (int y = 0; y < HLEN; y++)
            {
                for (int z = 0; z < DLEN; z++)
                {
                    cout << this->block(x, y, z);
                }
                cout << endl;
            }
            cout << endl;
        }
    }
};
