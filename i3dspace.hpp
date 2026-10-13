#pragma once

#include <array>
#include <iostream>
#include "include/raylib.h"
#include "i3dvector.hpp"
#include "blocks.hpp"

using namespace std;

template<int WLEN, int HLEN, int DLEN>
class Int3DSpace
{
private:
    WorldMTX mtx;
    const Voxel border{BORDER};

    static const int posAmountShort = 2;
    static const int posAmount = 4;
    const array<Vec3I, posAmountShort> verticalPosArr = // not really a "logic suit" compare to the other posArrays
    { 
        Vec3I{0,  1, 0},
        Vec3I{0, -1, 0}
    };
    const array<Vec3I, posAmount> sidePosArr =
    {
        Vec3I{ 0, 0, -1},
        Vec3I{-1, 0,  0},
        Vec3I{ 0, 0,  1},
        Vec3I{ 1, 0,  0}
    };
    const array<Vec3I, posAmount> sideFallPosArr =
    {
        Vec3I{ 0, -1, -1},
        Vec3I{-1, -1,  0},
        Vec3I{ 0, -1,  1},
        Vec3I{ 1, -1,  0}
    };
    const array<Vec3I, posAmount> diagPosArr =
    {
        Vec3I{ 1, 0, -1},
        Vec3I{-1, 0, -1},
        Vec3I{-1, 0,  1},
        Vec3I{ 1, 0,  1}
    };
    const array<Vec3I, posAmount> diagFallPosArr =
    {
        Vec3I{ 1, -1, -1},
        Vec3I{-1, -1, -1},
        Vec3I{-1, -1,  1},
        Vec3I{ 1, -1,  1}
    };

    Vector3 drawShift = {
        0.5f - WLEN / 2.0f,
        0.5f,
        0.5f - WLEN / 2.0f
    };
public:
    Int3DSpace() : mtx(WLEN, vector<vector<Voxel>>(HLEN, vector<Voxel>(DLEN, Voxel()))) {}



    int getWidth()  const { return WLEN; }

    int getHeight() const { return HLEN; }

    int getDepth()  const { return DLEN; }
    
    int getVolume() const { return WLEN*HLEN*DLEN; }



    WorldMTX & getWorld()             { return this->mtx; }

    const WorldMTX & getWorld() const { return this->mtx; }



    Voxel & voxel(int x, int y, int z)
    {
        return this->getWorld()[x][y][z];
    }

    Voxel & voxel(const Vec3I & v)
    {
        return this->voxel(v.x, v.y, v.z);
    }

    const Voxel & voxel(int x, int y, int z) const
    {
        if(x < 0 || WLEN <= x || y < 0 || HLEN <= y || z < 0 || DLEN <= z) return this->border;
        return this->getWorld()[x][y][z];
    }

    const Voxel & voxelConst(int x, int y, int z) const
    {
        return this->voxel(x, y, z);
    }
    
    const Voxel & voxel(const Vec3I & v) const
    {
        return this->voxel(v.x, v.y, v.z);
    }
    
    const Voxel & voxelConst(const Vec3I & v) const
    {
        return this->voxel(v);
    }



    BlockType & block(int x, int y, int z)                  { return (this->voxel(x, y, z)).block; }

    BlockType & block(const Vec3I & v)                      { return (this->voxel(v)).block; }

    const BlockType & block(int x, int y, int z)      const { return (this->voxel(x, y, z)).block; }

    const BlockType & blockConst(int x, int y, int z) const { return (this->voxel(x, y, z)).block; }
    
    const BlockType & block(const Vec3I & v)          const { return (this->voxel(v)).block; }
    
    const BlockType & blockConst(const Vec3I & v)     const { return (this->voxel(v)).block; }



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

    void createOneBlockOnTop(int xStart = 0, int xEnd = WLEN, int zStart = 0, int zEnd = DLEN)
    {
        this->block(rand() % (xStart - xEnd) + xStart, HLEN - 1, rand() % (zStart - zEnd) + zStart) = SAND;
    }



    void step()
    {
        // Le sable tombe.

        for (int y = 0; y < HLEN; y++)
            for (int x = 0; x < WLEN; x++)
                for (int z = 0; z < DLEN; z++)
                    this->voxelStep({x, y, z});
    }
    
    void voxelStep(const Vec3I & pos)
    {
        Voxel & voxel = this->voxel(pos);

        if (!voxel.canMove) return;

        switch(voxel.block)
        {
            case SAND:
                sandStep(pos, voxel);
                break;
        }
    }

    void sandStep(const Vec3I & pos, Voxel & voxel)
    {
        BlockType & block = voxel.block;

        { // The block tries to fall straight down
            const Vec3I underBlockCoord = pos - verticalPosArr[0];

            if (this->blockConst(underBlockCoord) == AIR)
            {
                this->block(underBlockCoord) = SAND;
                block = AIR;
                activateNeighboursVisibility(pos);
                return;
            }
        }

        vector<int> validPosIndexes; // The block checks which horizontal directions it is allowed to move in
        for (int i = 0; i < this->posAmount; i++)
            if (this->blockConst(pos + sidePosArr[i]) == AIR)
                validPosIndexes.push_back(i);

        int validPosAmount = validPosIndexes.size(); // The block counts the horizontal directions it is allowed to move in

        if (validPosAmount == 0) // The block is surrourded by 4 solid blocks
        {
            voxel.canMove = false;
            return;
        }

        int seed = rand(); // randomization of direction order the block will try to move in
        for (int i = 0; i < validPosAmount - 1; i++)
        {
            int tmp = validPosIndexes[i];
            validPosIndexes[i] = validPosIndexes[seed % (validPosAmount - i) + i];
            validPosIndexes[seed % (validPosAmount - i) + i] = tmp;
        }
        
        for (int i : validPosIndexes) // The block tries to fall sideways
        {
            const Vec3I sideFallBlockCoord = pos + sideFallPosArr[i];

            if (this->blockConst(sideFallBlockCoord) == AIR)
            {
                this->block(sideFallBlockCoord) = SAND;
                block = AIR;
                activateNeighboursVisibility(pos);
                return;
            }
        }
        
        for (int i : validPosIndexes) // The block tries to move sideways
        {
            if (this->blockConst(pos + diagFallPosArr[i]) == AIR || this->blockConst(pos + diagFallPosArr[(i+1)%this->posAmount]) == AIR)
            {
                Vec3I sideBlockCoord = pos + sidePosArr[i];
                this->block(sideBlockCoord) = SAND;
                block = AIR;
                activateNeighboursVisibility(pos);
                return;
            }
        }

        voxel.canMove = false; // The block didn't have any move opportunities
    }



    bool isVoxelVisible(const Vec3I & pos) const
    {
        if (pos.x == 0 || pos.x == WLEN - 1 ||
            pos.y == 0 || pos.y == HLEN - 1 ||
            pos.z == 0 || pos.z == DLEN - 1)
            return true;

        return // test each neighbors up down and direct lateral side. if one neighbors is AIR, show voxel.
            blockConst(pos + sidePosArr[0]) == AIR ||
            blockConst(pos + sidePosArr[1]) == AIR ||
            blockConst(pos + sidePosArr[2]) == AIR ||
            blockConst(pos + sidePosArr[3]) == AIR ||
            blockConst(pos + verticalPosArr[0]) == AIR ||
            blockConst(pos + verticalPosArr[1]) == AIR;
    }

    void activateNeighboursVisibility(const Vec3I & pos)
    {
        for (const Vec3I & verticalPos : this->verticalPosArr)
        {
            Vec3I neighbourPos = pos + verticalPos;
            if (0 <= neighbourPos.x && neighbourPos.x < WLEN && 0 <= neighbourPos.y && neighbourPos.y < HLEN && 0 <= neighbourPos.z && neighbourPos.z < DLEN)
                this->voxel(neighbourPos).isVisible = true;
        }

        for (const Vec3I & sidePos     : this->sidePosArr)
        {
            Vec3I neighbourPos = pos + sidePos;
            if (0 <= neighbourPos.x && neighbourPos.x < WLEN && 0 <= neighbourPos.y && neighbourPos.y < HLEN && 0 <= neighbourPos.z && neighbourPos.z < DLEN)
                this->voxel(neighbourPos).isVisible = true;
        }
    }

    void updateWorldVoxelVisibility()
    {
        for (int y = 0; y < HLEN; y++)
            for (int x = 0; x < WLEN; x++)
                for (int z = 0; z < DLEN; z++)
                {
                    this->voxel(x, y, z).isVisible = this->isVoxelVisible({ x, y, z });
                }
    }



    // sandAmount        = blockAmountArr[0];
    // staticBlockAmount = blockAmountArr[1];
    void countBlockStats(array<int, 2> & blockAmountArr) const
    {
        for (int y = 0; y < HLEN; y++)
            for (int x = 0; x < WLEN; x++)
                for (int z = 0; z < DLEN; z++) 
                {
                    const BlockType & currentBt = this->blockConst(x,y,z);

                    if (currentBt == SAND) blockAmountArr[0]++;
                    else if (currentBt != AIR) blockAmountArr[1]++;
                }
    }



    void drawWorld() const
    {
        for (int y = 0; y < HLEN; y++)
            for (int x = 0; x < WLEN; x++)
                for (int z = 0; z < DLEN; z++)
                    if (this->voxelConst(x, y, z).isVisible)
                        this->drawVoxel({ x, y, z });
    }

    void drawVoxel(const Vec3I & pos) const
    {
        Vector3 position = pos + this->drawShift;

        switch (this->block(pos))
        {
            case DIRT:
                DrawCube(position, 1.0f, 1.0f, 1.0f, BROWN);
                break;
            case SAND:
                DrawCube(position, 1.0f, 1.0f, 1.0f, YELLOW);
                DrawCubeWires(position, 1.0f, 1.0f, 1.0f, BROWN);
                break;
            case STONE:
                DrawCube(position, 1.0f, 1.0f, 1.0f, GRAY);
                break;
            case WOOD:
                DrawCube(position, 1.0f, 1.0f, 1.0f, BROWN);
                DrawCubeWires(position, 1.0f, 1.0f, 1.0f, BLACK);
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
