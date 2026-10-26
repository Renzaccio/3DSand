#pragma once

#include "blocks.hpp"
#include "i3dvector.hpp"
#include "furnitures.hpp"
#include <unordered_map>

using namespace std;

typedef vector<Voxel> WorldMTX;

template<int WLEN, int HLEN, int DLEN>
struct Int3DSpace
{
private:

    WorldMTX mtx;
    const Voxel border{ BORDER };
    unordered_map<int, VoxelLogicalGroup> staticVoxelsGroups;

    WorldMTX & getWorld()             { return this->mtx; }
    const WorldMTX & getWorld() const { return this->mtx; }

    int clock = 0;

    static const int posAmount = 4;
    const array<Vec3I, posAmount> sidePosArr =
    {
        RIGHTVOXEL,
        FRONTVOXEL,
        LEFTVOXEL,
        BACKVOXEL
    };
    const array<Vec3I, posAmount> sideFallPosArr =
    {
        RIGHTVOXEL + LOWERVOXEL,
        FRONTVOXEL + LOWERVOXEL,
        LEFTVOXEL  + LOWERVOXEL,
        BACKVOXEL  + LOWERVOXEL
    };
    const array<Vec3I, posAmount> diagPosArr =
    {
        RIGHTVOXEL + BACKVOXEL,
        RIGHTVOXEL + FRONTVOXEL,
        LEFTVOXEL  + FRONTVOXEL,
        LEFTVOXEL  + BACKVOXEL,
    };
    const array<Vec3I, posAmount> diagFallPosArr =
    {
        RIGHTVOXEL + BACKVOXEL  + LOWERVOXEL,
        RIGHTVOXEL + FRONTVOXEL + LOWERVOXEL,
        LEFTVOXEL  + FRONTVOXEL + LOWERVOXEL,
        LEFTVOXEL  + BACKVOXEL  + LOWERVOXEL,
    };

    Vector3 drawShift = {
        0 - WLEN * 0.5f,
        0,
        0 - WLEN * 0.5f,
    };

public:

    //// BASE FUNCTIONS ////
    Int3DSpace() : mtx{ HLEN*WLEN*DLEN } {} // Constructor

    Vector3 getDrawShift() const { return this->drawShift; }

    int getWidth()  const { return WLEN; }

    int getHeight() const { return HLEN; }

    int getDepth()  const { return DLEN; }

    int getVolume() const { return WLEN * HLEN * DLEN; }



    bool isInBounds(int x, int y, int z) const { return 0 <= x && x < WLEN && 0 <= y && y < HLEN && 0 <= z && z < DLEN; }

    bool isInBounds(const Vec3I & pos) const { return this->isInBounds(pos.x, pos.y, pos.z); }



    Voxel & operator()(size_t x, size_t y, size_t z)
    {
        return this->mtx[x * HLEN * DLEN + y * DLEN + z];
    }

    const Voxel & operator()(size_t x, size_t y, size_t z) const
    {
        return this->mtx[x * HLEN * DLEN + y * DLEN + z];
    }

    Voxel & voxel(int x, int y, int z)
    {
        return (*this)(x,y,z);
    }

    Voxel & voxel(const Vec3I & v)
    {
        return this->voxel(v.x, v.y, v.z);
    }

    const Voxel & voxel(int x, int y, int z) const
    {
        if (this->isInBounds(x, y, z)) return (*this)(x,y,z);
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

    const BlockType & getBlock(const Vector3 & v)     const { return (this->voxel(toVec3I((Vector3) { v - drawShift} )).block); }
    //// End of BASE FUNCTIONS ////



    //// WORLD MODIFICATION ////
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

    void placeOneBlockOnTop(int xStart = 0, int xEnd = WLEN, int zStart = 0, int zEnd = DLEN, BlockType block = SAND)
    {
        this->block(rand() % (xStart - xEnd) + xStart, HLEN - 1, rand() % (zStart - zEnd) + zStart) = block;
    }

    void addNewStaticStructure(VoxelLogicalGroup group)
    {
        for (LogicalVoxel lv : group.mask)
        {
            Vec3I c = group.origin + lv.localPos;
            Voxel & currentVoxel = this->voxel(c);
            currentVoxel.block = lv.type;
            if ((currentVoxel.block != WATER) && (currentVoxel.block != SAND))
                currentVoxel.group = group.id;
            this->activateAllNeighboursSurfaceVisibility(c);
        }

        this->staticVoxelsGroups[group.id] = group;
    }
    //// End of WORLD MODIFICATION ////



    //// SIMULATION ////
    void step()
    {
        // the blocks fall.

        switch (((rand() % 8) + clock) % 8) {
            case 0:
                for (int y = 0; y < HLEN; y++) for (int x = 0; x < WLEN; x++)      for (int z = 0; z < DLEN; z++)
                    this->voxelStep({ x, y, z });
                break;
            case 1:
                for (int y = 0; y < HLEN; y++) for (int x = 0; x < WLEN; x++)      for (int z = DLEN - 1; z >= 0; z--)
                    this->voxelStep({ x, y, z });
                break;
            case 2:
                for (int y = 0; y < HLEN; y++) for (int x = WLEN - 1; x >= 0; x--) for (int z = 0; z < DLEN; z++)
                    this->voxelStep({ x, y, z });
                break;
            case 3:
                for (int y = 0; y < HLEN; y++) for (int x = WLEN - 1; x >= 0; x--) for (int z = DLEN - 1; z >= 0; z--)
                    this->voxelStep({ x, y, z });
                break;
            case 4:
                for (int y = 0; y < HLEN; y++) for (int z = 0; z < DLEN; z++)      for (int x = 0; x < WLEN; x++)
                    this->voxelStep({ x, y, z });
                break;
            case 5:
                for (int y = 0; y < HLEN; y++) for (int z = 0; z < DLEN; z++)      for (int x = WLEN - 1; x >= 0; x--)
                    this->voxelStep({ x, y, z });
                break;
            case 6:
                for (int y = 0; y < HLEN; y++) for (int z = DLEN - 1; z >= 0; z--) for (int x = 0; x < WLEN; x++)
                    this->voxelStep({ x, y, z });
                break;
            case 7:
                for (int y = 0; y < HLEN; y++) for (int z = DLEN - 1; z >= 0; z--) for (int x = WLEN - 1; x >= 0; x--)
                    this->voxelStep({ x, y, z });
                break;
        }

        this->clock++;
    }

    void voxelStep(const Vec3I & pos)
    {
        Voxel & voxel = this->voxel(pos);

        if ((voxel.group >= 0) && (voxel.block != WATER) && (voxel.block != SAND))
            return;

        if (!voxel.canMove) return;

        switch(voxel.block)
        {
            case SAND:
                this->sandStep(pos, voxel);
                break;
            case WATER:
                this->waterStep(pos, voxel);
                break;
            default:
                break;
        }
    }

    void sandStep(const Vec3I & pos, Voxel & voxel)
    {
        BlockType & block = voxel.block;

        if (SAND < this->blockConst(pos + LOWERVOXEL)) // The sand block tries to fall straight down
        {
            this->swapBlocks(pos, SAND, pos + LOWERVOXEL, this->block(pos + LOWERVOXEL));
            return;
        }

        vector<int> validPosIndexVec; // The sand block checks which horizontal directions it is allowed to move in
        for (int i = 0; i < this->posAmount; i++)
            if (SAND < this->blockConst(pos + this->sidePosArr[i]))
                validPosIndexVec.push_back(i);

        if (validPosIndexVec.size() == 0) // The sand block is surrourded by 4 solid blocks
        {
            voxel.canMove = false;
            return;
        }

        this->shuffle(validPosIndexVec); // randomization of direction order the sand block will try to move in

        for (int posIndex : validPosIndexVec) // The sand block tries to fall sideways
        {
            if (SAND < this->blockConst(pos + this->sideFallPosArr[posIndex]))
            {
                const Vec3I & sideFallBlockCoord = pos + this->sideFallPosArr[posIndex];
                this->swapBlocks(pos, SAND, sideFallBlockCoord, this->block(sideFallBlockCoord));
                return;
            }
        }

        if (rand() % 10 >= 1) return;

        for (int posIndex : validPosIndexVec) // The sand block tries to move sideways
        {
            if (SAND < this->blockConst(pos + this->diagFallPosArr[posIndex]) || SAND < this->blockConst(pos + this->diagFallPosArr[(posIndex + 1) % this->posAmount]))
            {
                const Vec3I & sideBlockCoord = pos + this->sidePosArr[posIndex];
                this->swapBlocks(pos, SAND, sideBlockCoord, this->block(sideBlockCoord));
                return;
            }
        }

        voxel.canMove = false; // The sand block didn't have any move opportunities
    }

    void waterStep(const Vec3I & pos, Voxel & voxel)
    {
        BlockType & block = voxel.block;

        if (this->blockConst(pos + LOWERVOXEL) == AIR) // The water block tries to fall straight down
        {
            this->swapBlocks(pos, WATER, pos + LOWERVOXEL, AIR);
            return;
        }

        vector<int> validPosIndexVec; // The water block checks which horizontal directions it is allowed to move in
        for (int i = 0; i < this->posAmount; i++)
            if (this->blockConst(pos + this->sidePosArr[i]) == AIR)
                validPosIndexVec.push_back(i);

        if (validPosIndexVec.size() == 0) // The water block is surrourded by 4 solid blocks
        {
            voxel.canMove = false;
            return;
        }

        this->shuffle(validPosIndexVec); // randomization of direction order the water block will try to move in

        for (int posIndex : validPosIndexVec) // The water block tries to fall sideways
        {
            if (this->blockConst(pos + this->sideFallPosArr[posIndex]) == AIR)
            {
                this->swapBlocks(pos, WATER, pos + this->sideFallPosArr[posIndex], AIR);
                return;
            }
        }

        if (rand() % 6 >= 5) return;

        for (int posIndex : validPosIndexVec) // The water block tries to move sideways
        {
            if (this->blockConst(pos + this->sidePosArr[posIndex]) == AIR)
            {
                this->swapBlocks(pos, WATER, pos + this->sidePosArr[posIndex], AIR);
                return;
            }
        }

        voxel.canMove = false; // The block didn't have any move opportunities
    }

    void shuffle(vector<int> & vector)
    {
        int size = vector.size();
        int seed = rand();
        for (int i = 0; i < size - 1; i++)
        {
            int i1 = (i + clock) % size;
            int i2 = ((seed % (size - i) + i) % size + clock) % size;
            int tmpIndex = vector[i1];
            vector[i1]   = vector[i2];
            vector[i2]   = tmpIndex;
        }
    }

    void swapBlocks(const Vec3I & origin, BlockType blkMoving, const Vec3I & destination, BlockType blkMoved)
    {
        this->updateBlock(destination, blkMoving);
        this->activateMobility(destination);

        this->updateBlock(origin, blkMoved);
        this->activateNeighboursMobility(origin);
    }

    void updateBlock(const Vec3I & pos, BlockType block)
    {
        this->block(pos) = block;
        this->activateAllNeighboursSurfaceVisibility(pos);
    }

    void activateNeighboursMobility(const Vec3I & pos)
    {
        this->activateMobility(pos);
        this->activateMobility(pos + RIGHTVOXEL);
        this->activateMobility(pos +  LEFTVOXEL);
        this->activateMobility(pos + FRONTVOXEL);
        this->activateMobility(pos +  BACKVOXEL);
        for(int x = -1; x <= 1; x++)
            for(int z = -1; z <= 1; z++)
                this->activateMobility(pos + Vec3I{ x, 1, z });
    }

    void activateMobility(const Vec3I & pos)
    {
        if (this->isInBounds(pos)) this->voxel(pos).canMove = true;
    }
    //// End of SIMULATION ////



    //// VISIBILITY ////
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
            blockClass != this->voxel(pos + RIGHTVOXEL).getBlockClass() || blockClass != this->voxel(pos +  LEFTVOXEL).getBlockClass() ||
            blockClass != this->voxel(pos + UPPERVOXEL).getBlockClass() || blockClass != this->voxel(pos + LOWERVOXEL).getBlockClass() ||
            blockClass != this->voxel(pos + FRONTVOXEL).getBlockClass() || blockClass != this->voxel(pos +  BACKVOXEL).getBlockClass();
    }

    void activateAllNeighboursSurfaceVisibility(const Vec3I & pos)
    {
        this->activateSurfaceVisibility(pos + RIGHTVOXEL); this->activateSurfaceVisibility(pos +  LEFTVOXEL);
        this->activateSurfaceVisibility(pos + UPPERVOXEL); this->activateSurfaceVisibility(pos + LOWERVOXEL);
        this->activateSurfaceVisibility(pos + FRONTVOXEL); this->activateSurfaceVisibility(pos +  BACKVOXEL);
    }

    void activateSurfaceVisibility(const Vec3I & pos)
    {
        if (this->isInBounds(pos)) this->voxel(pos).hasVisibleSurface = true;
    }
    //// End of VISIBILITY ////



    // sandAmount        = blockAmountArr[0];
    // staticBlockAmount = blockAmountArr[1];
    void countBlockStats(pair<int, int> & blockAmounts) const
    {
        for (int y = 0; y < HLEN; y++)
            for (int x = 0; x < WLEN; x++)
                for (int z = 0; z < DLEN; z++)
                {
                    const Voxel & voxel = this->voxel(x,y,z);

                    switch (voxel.block) {
                        default :
                            blockAmounts.first++;
                            break;
                        case SAND :
                        case WATER :
                            if (voxel.canMove) blockAmounts.second++;
                            else               blockAmounts.first++;
                            break;
                        case AIR :
                            break;
                    }
                }
    }



    //// DRAWING ////
    void drawWorld() const
    {
        for (int x = 0; x <= WLEN; x++) for (int y = 0; y <= HLEN; y++) for (int z = 0; z <= DLEN; z++)
            this->DrawSolidVoxel({ x, y, z });

        for (int x = 0; x <= WLEN; x++) for (int y = 0; y <= HLEN; y++) for (int z = 0; z <= DLEN; z++)
            this->DrawWaterVoxel({ x, y, z });
    }

    void DrawSolidVoxel(const Vec3I & pos) const
    {
        const Voxel & voxel = this->voxel(pos);
        if (!voxel.hasVisibleSurface || voxel.getBlockClass() != SOLID) return;
        drawVoxel(pos, voxel);
    }

    void DrawWaterVoxel(const Vec3I & pos) const
    {
        const Voxel & voxel = this->voxel(pos);
        if (!voxel.hasVisibleSurface || voxel.block != WATER) return;
        drawVoxel(pos, voxel);
    }

    void drawVoxel(const Vec3I & pos, const Voxel & voxel) const
    {
        Vector3 drawOriginPos = pos + this->drawShift;
        this->drawVoxelFace(drawOriginPos, voxel, pos + RIGHTVOXEL, RIGHT);
        this->drawVoxelFace(drawOriginPos, voxel, pos +  LEFTVOXEL, LEFT);
        this->drawVoxelFace(drawOriginPos, voxel, pos + LOWERVOXEL, BOTTOM);
        this->drawVoxelFace(drawOriginPos, voxel, pos + UPPERVOXEL, TOP);
        this->drawVoxelFace(drawOriginPos, voxel, pos + FRONTVOXEL, FRONT);
        this->drawVoxelFace(drawOriginPos, voxel, pos +  BACKVOXEL, BACK);
    }

    void drawVoxelFace(const Vector3 & drawOriginPos, const Voxel & voxel, const Vec3I & adjacentPos, Direction face) const
    {
        const Voxel & adjacentVoxel = this->voxel(adjacentPos);
        if (adjacentVoxel.hasVisibleSurface)
            voxel.drawFace(drawOriginPos, adjacentVoxel.block, face);
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
    //// End of DRAWING ////

};
