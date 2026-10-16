#pragma once

#include <array>
#include <iostream>
#include <unordered_map>
#include "../raylib/include/raylib.h"
// #include "../raylib/include/raymath.h"
#include "i3dvector.hpp"

using namespace std;

enum BlockType { BORDER, DEBUG, STONE, WOOD, DIRT, SAND, AIR };

enum BlockClass { SOLID, LIQUID, GAS };

enum Direction { RIGHT, LEFT, TOP, BOTTOM, FRONT, BACK };

inline ostream & operator << (ostream & stream, BlockType block)
{
    switch (block) {
        case BORDER:
            stream << "BORDER"; break;
        case DEBUG:
            stream << "DEBUG";  break;
        case STONE:
            stream << "STONE";  break;
        case WOOD:
            stream << "WOOD";   break;
        case DIRT:
            stream << "DIRT";   break;
        case SAND:
            stream << "SAND";   break;
        case AIR:
            stream << "AIR";    break;
    }

    return stream;
}

struct Voxel
{
private:

    const array<Vec3I, 8> verticesPosArr { {
        { 0, 0, 0 },
        { 0, 0, 1 },
        { 0, 1, 0 },
        { 0, 1, 1 },
        { 1, 0, 0 },
        { 1, 0, 1 },
        { 1, 1, 0 },
        { 1, 1, 1 },
    } };

    const unordered_map<Direction, array<int, 4>> faceIndexesMap {
        { RIGHT,  { 0, 1, 3, 2 } },
        { LEFT,   { 4, 6, 7, 5 } },
        { BOTTOM, { 0, 4, 5, 1 } },
        { TOP,    { 2, 3, 7, 6 } },
        { FRONT,  { 0, 2, 6, 4 } },
        { BACK,   { 1, 5, 7, 3 } },
    };

public:

    BlockType block;
    bool canMove           = true;
    bool hasVisibleSurface = true;


    Voxel() : block{ AIR } {}
    Voxel(BlockType bl) : block{ bl } {}


    BlockClass getBlockClass() const
    {
        switch (this->block) {
            default:
                return SOLID;
            case BORDER:
            case AIR:
                return GAS;
        }
    }

    bool isSurfaceVisible(BlockType adjacentBlock) const
    {
        switch (this->block)
        {
            case BORDER:
            case AIR:
                return false;
            default:
                switch(adjacentBlock)
                {
                    case BORDER:
                    case AIR:
                        return true;
                    default:
                        return false;
                }
        }
    }

    Color getColor() const
    {
        switch (this->block)
        {
            case DEBUG:
                return PURPLE;
            case STONE:
                return GRAY;
            case WOOD:
            case DIRT:
                return BROWN;
            case SAND:
                return YELLOW;
            default:
                return BLANK;
        }
    }

    void drawFace(const Vector3 & position, BlockType adjacentBlock, Direction face) const
    {
        if (this->isSurfaceVisible(adjacentBlock))
        {
            const Color & color = this->getColor();
            const array<int, 4> & faceIndexes = faceIndexesMap.find(face)->second;
            DrawTriangle3D(position + this->verticesPosArr[faceIndexes[0]], position + this->verticesPosArr[faceIndexes[1]], position + this->verticesPosArr[faceIndexes[3]], color);
            DrawTriangle3D(position + this->verticesPosArr[faceIndexes[1]], position + this->verticesPosArr[faceIndexes[2]], position + this->verticesPosArr[faceIndexes[3]], color);
        }
    }

    void drawLines(const Vector3 position) const
    {
        switch (this->block)
        {
            case DEBUG:
                DrawCubeWires(position, 1.0f, 1.0f, 1.0f, ORANGE);
                break;
            case WOOD:
                DrawCubeWires(position, 1.0f, 1.0f, 1.0f, BLACK);
                break;
            case SAND:
                DrawCubeWires(position, 1.0f, 1.0f, 1.0f, BROWN);
                break;
            default:
                break;
        }
    }

    void draw(const Vector3 position) const
    {
        DrawCube(position, 1.0f, 1.0f, 1.0f, this->getColor());
        this->drawLines(position);
    }
};