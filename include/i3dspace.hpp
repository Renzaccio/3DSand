#pragma once

#include "blocks.hpp"

using namespace std;

typedef vector<vector<vector<Voxel>>> WorldMTX;

template<int WLEN, int HLEN, int DLEN>
struct Int3DSpace
{
private:

    WorldMTX mtx;
    const Voxel border{ BORDER };

    WorldMTX & getWorld()             { return this->mtx; }
    const WorldMTX & getWorld() const { return this->mtx; }

    static const int posAmountShort = 2;
    static const int posAmount = 4;
    const array<Vec3I, posAmountShort> verticalPosArr = // not really a "logic suit" compare to the other posArrays
    {
        Vec3I{ 0, -1, 0 },
        Vec3I{ 0,  1, 0 }
    };
    const array<Vec3I, posAmount> sidePosArr =
    {
        Vec3I{  0, 0, -1 },
        Vec3I{ -1, 0,  0 },
        Vec3I{  0, 0,  1 },
        Vec3I{  1, 0,  0 }
    };
    const array<Vec3I, posAmount> sideFallPosArr =
    {
        Vec3I{  0, -1, -1 },
        Vec3I{ -1, -1,  0 },
        Vec3I{  0, -1,  1 },
        Vec3I{  1, -1,  0 }
    };
    const array<Vec3I, posAmount> diagPosArr =
    {
        Vec3I{  1, 0, -1 },
        Vec3I{ -1, 0, -1 },
        Vec3I{ -1, 0,  1 },
        Vec3I{  1, 0,  1 }
    };
    const array<Vec3I, posAmount> diagFallPosArr =
    {
        Vec3I{  1, -1, -1 },
        Vec3I{ -1, -1, -1 },
        Vec3I{ -1, -1,  1 },
        Vec3I{  1, -1,  1 }
    };
    Vector3 drawShift = {
        0 - WLEN * 0.5f,
        0,
        0 - WLEN * 0.5f,
    };
public:
    Int3DSpace() : mtx{ WLEN, vector<vector<Voxel>>{ HLEN, vector<Voxel>{ DLEN, Voxel{} } } } {}

    Vector3 getDrawShift() const { return this->drawShift; }

    int getWidth()  const { return WLEN; }

    int getHeight() const { return HLEN; }

    int getDepth()  const { return DLEN; }

    int getVolume() const { return WLEN*HLEN*DLEN; }

    bool isInBounds(int x, int y, int z) const { return 0 <= x && x < WLEN && 0 <= y && y < HLEN && 0 <= z && z < DLEN; }

    bool isInBounds(const Vec3I & pos) const { return this->isInBounds(pos.x, pos.y, pos.z); }



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
        if (this->isInBounds(x, y, z)) return this->getWorld()[x][y][z];
        return this->border;
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



    void placeBlock(Vec3I pos, BlockType block)
    {
        if (!(this->isInBounds(pos)))
        {
            cout << "OoB when trying to place a " << block << " block at " << pos << endl;
            return;
        }

        this->block(pos) = block;
    }

    void placeParallelepiped(Vec3I startPos, Vec3I endPos, BlockType block)
    {
        if (!(this->isInBounds(startPos) && this->isInBounds(endPos)))
        {
            cout << "OoB when trying to place a " << block << " parallelepiped from " << startPos << " to " << endPos << endl;
            return;
        }

        for (int x = startPos.x; x <= endPos.x; x++)
            for (int y = startPos.y; y <= endPos.y; y++)
                for (int z = startPos.z; z <= endPos.z; z++)
                    this->block(x, y, z) = block;
    }

    void placeFlatFloor(int y, BlockType block)
    {
        this->placeParallelepiped({ 0, y, 0 }, { WLEN - 1, y, DLEN - 1 }, block);
    }

    void placeOneBlockOnTop(int xStart = 0, int xEnd = WLEN, int zStart = 0, int zEnd = DLEN)
    {
        this->block(rand() % (xStart - xEnd) + xStart, HLEN - 1, rand() % (zStart - zEnd) + zStart) = SAND;
    }

    void placeChair3x4x3(const Vec3I & origin)
    {
        //pieds
        this->placeBlock(origin + Vec3I{ 0, 0, 0 }, WOOD);
        this->placeBlock(origin + Vec3I{ 0, 0, 2 }, WOOD);
        this->placeBlock(origin + Vec3I{ 2, 0, 0 }, WOOD);
        this->placeBlock(origin + Vec3I{ 2, 0, 2 }, WOOD);

        //plateau
        this->placeParallelepiped(origin + Vec3I{ 0, 1, 0 }, origin + Vec3I{ 2, 1, 2 }, WOOD);

        //dossier
        this->placeParallelepiped(origin + Vec3I{ 0, 2, 2 }, origin + Vec3I{ 2, 3, 2 }, WOOD);
    }

    void placeTable5x4x5(const Vec3I & origin)
    {
        //pieds
        this->placeParallelepiped(origin + Vec3I{ 0, 0, 0 }, origin + Vec3I{ 0, 2, 0 }, STONE);
        this->placeParallelepiped(origin + Vec3I{ 0, 0, 4 }, origin + Vec3I{ 0, 2, 4 }, STONE);
        this->placeParallelepiped(origin + Vec3I{ 4, 0, 0 }, origin + Vec3I{ 4, 2, 0 }, STONE);
        this->placeParallelepiped(origin + Vec3I{ 4, 0, 4 }, origin + Vec3I{ 4, 2, 4 }, STONE);

        //plateau
        this->placeParallelepiped(origin + Vec3I{ 0, 3, 0 }, origin + Vec3I{ 4, 3, 4 }, WOOD);
    }



    void step()
    {
        // Le sable tombe.

        for (int y = 0; y < HLEN; y++)
            for (int x = 0; x < WLEN; x++)
                for (int z = 0; z < DLEN; z++)
                    this->voxelStep({ x, y, z });
    }

    void voxelStep(const Vec3I & pos)
    {
        Voxel & voxel = this->voxel(pos);

        if (!voxel.canMove) return;

        switch(voxel.block)
        {
            case SAND:
                this->sandStep(pos, voxel);
                break;
            default:
                break;
        }
    }

    void sandStep(const Vec3I & pos, Voxel & voxel)
    {
        BlockType & block = voxel.block;

        { // The block tries to fall straight down
            const Vec3I underBlockCoord = pos + this->verticalPosArr[0];

            if (this->blockConst(underBlockCoord) == AIR)
            {
                this->updateBlock(underBlockCoord, SAND);
                this->updateBlock(pos, AIR);
                return;
            }
        }

        vector<int> validPosIndexes; // The block checks which horizontal directions it is allowed to move in
        for (int i = 0; i < this->posAmount; i++)
            if (this->blockConst(pos + this->sidePosArr[i]) == AIR)
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
            const Vec3I sideFallBlockCoord = pos + this->sideFallPosArr[i];

            if (this->blockConst(sideFallBlockCoord) == AIR)
            {
                this->updateBlock(sideFallBlockCoord, SAND);
                this->updateBlock(pos, AIR);
                return;
            }
        }

        for (int i : validPosIndexes) // The block tries to move sideways
        {
            if (this->blockConst(pos + diagFallPosArr[i]) == AIR || this->blockConst(pos + diagFallPosArr[(i+1)%this->posAmount]) == AIR)
            {
                this->updateBlock(pos + this->sidePosArr[i], SAND);
                this->updateBlock(pos, AIR);
                return;
            }
        }

        voxel.canMove = false; // The block didn't have any move opportunities
    }

    void updateBlock(const Vec3I & pos, BlockType block)
    {
        this->block(pos) = block;
        this->activateAllNeighboursSurfaceVisibility(pos);
    }



    void updateWorldVoxelSurfaceVisibility()
    {
        for (int y = 0; y < HLEN; y++)
            for (int x = 0; x < WLEN; x++)
                for (int z = 0; z < DLEN; z++)
                { // This function only updates the visibility of blocks already visible. Hidden blocks are already updated inside step()
                    bool & voxelHasVisibleSurface = this->voxel(x, y, z).hasVisibleSurface;
                    if (voxelHasVisibleSurface)
                        voxelHasVisibleSurface = this->voxelHasVisibleSurface({ x, y, z });
                }
    }

    bool voxelHasVisibleSurface(const Vec3I & pos) const
    {
        BlockClass blockClass = this->voxel(pos).getBlockClass();

        return // test each neighbors up down and direct lateral side. if one neighbors is AIR, show voxel.
            blockClass != this->voxel(pos + this->sidePosArr[0]).getBlockClass() ||
            blockClass != this->voxel(pos + this->sidePosArr[1]).getBlockClass() ||
            blockClass != this->voxel(pos + this->sidePosArr[2]).getBlockClass() ||
            blockClass != this->voxel(pos + this->sidePosArr[3]).getBlockClass() ||
            blockClass != this->voxel(pos + this->verticalPosArr[0]).getBlockClass() ||
            blockClass != this->voxel(pos + this->verticalPosArr[1]).getBlockClass();
    }

    void activateAllNeighboursSurfaceVisibility(const Vec3I & pos)
    {
        for (const Vec3I & verticalPos : this->verticalPosArr) this->activateNeighbourSurfaceVisibility(pos + verticalPos);
        for (const Vec3I & sidePos     : this->sidePosArr)     this->activateNeighbourSurfaceVisibility(pos + sidePos);
    }

    void activateNeighbourSurfaceVisibility(const Vec3I & neighbourPos)
    {
        if (this->isInBounds(neighbourPos)) this->voxel(neighbourPos).hasVisibleSurface = true;
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
        for (int x = 0; x <= WLEN; x++)
            for (int y = 0; y <= HLEN; y++)
                for (int z = 0; z <= DLEN; z++)
                    this->drawVoxel({ x, y, z });
    }

    void drawVoxel(const Vec3I & pos) const
    {
        const Voxel & voxel = this->voxel(pos);
        Vector3 drawPos = pos + this->drawShift;

        if (voxel.isTargeted)
            // Faces use the minimum corner; DrawCubeWires expects the center
            DrawCubeWires(drawPos + Vector3{ 0.5f, 0.5f, 0.5f }, 1.0f, 1.0f, 1.0f, BLACK);

        if (!voxel.hasVisibleSurface || voxel.getBlockClass() == GAS)
            return;

        {
            const Voxel & adjacentVoxel = this->voxel(pos + this->sidePosArr[1]);
            if (adjacentVoxel.hasVisibleSurface)
                voxel.drawFace(drawPos, adjacentVoxel.block, RIGHT);
        }
        {
            const Voxel & adjacentVoxel = this->voxel(pos + this->sidePosArr[3]);
            if (adjacentVoxel.hasVisibleSurface)
                voxel.drawFace(drawPos, adjacentVoxel.block, LEFT);
        }
        {
            const Voxel & adjacentVoxel = this->voxel(pos + this->verticalPosArr[0]);
            if (adjacentVoxel.hasVisibleSurface)
                voxel.drawFace(drawPos, adjacentVoxel.block, BOTTOM);
        }
        {
            const Voxel & adjacentVoxel = this->voxel(pos + this->verticalPosArr[1]);
            if (adjacentVoxel.hasVisibleSurface)
                voxel.drawFace(drawPos, adjacentVoxel.block, TOP);
        }
        {
            const Voxel & adjacentVoxel = this->voxel(pos + this->sidePosArr[0]);
            if (adjacentVoxel.hasVisibleSurface)
                voxel.drawFace(drawPos, adjacentVoxel.block, FRONT);
        }
        {
            const Voxel & adjacentVoxel = this->voxel(pos + this->sidePosArr[2]);
            if (adjacentVoxel.hasVisibleSurface)
                voxel.drawFace(drawPos, adjacentVoxel.block, BACK);
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