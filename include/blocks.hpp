#pragma once

#include <array>
#include <unordered_map>
#include "../raylib/include/raylib.h"
#include "i3dvector.hpp"

using namespace std;

enum BlockType { BORDER, DEBUG, STONE, WOOD, DIRT, SAND, WATER, AIR };

enum BlockClass { SOLID, LIQUID, GAS };

enum Direction { RIGHT, LEFT, TOP, BOTTOM, FRONT, BACK };

inline ostream & operator << (ostream & stream, BlockType block)
{
    switch (block) {
        case BORDER: stream << "BORDER"; break;
        case DEBUG:  stream << "DEBUG";  break;
        case STONE:  stream << "STONE";  break;
        case WOOD:   stream << "WOOD";   break;
        case DIRT:   stream << "DIRT";   break;
        case SAND:   stream << "SAND";   break;
        case WATER:  stream << "WATER";  break;
        case AIR:    stream << "AIR";    break;
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
    bool isTargeted        = false;
    int group              = -1;


    Voxel() :              block{ AIR } {}
    Voxel(BlockType blk) : block{ blk } {}



    //// GETTERS ////
    BlockClass getBlockClass() const
    {
        switch (this->block) {
            default:
                return SOLID;
            case WATER:
                return LIQUID;
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
            case WATER:
                switch(adjacentBlock)
                {
                    case BORDER:
                    case AIR:
                        return true;
                    default:
                        return false;
                }
            default:
                switch(adjacentBlock)
                {
                    case BORDER:
                    case AIR:
                    case WATER:
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
            case WATER:
                return Color{ 0, 121, 241, 192 };
            case SAND:
                return YELLOW;
            default:
                return BLANK;
        }
    }

    unsigned char getLuminosity(Direction face) const
    {
        switch (face)
        {
            case TOP:
                return 255;
            case FRONT:
            case BACK:
                return 216;
            case RIGHT:
            case LEFT:
                return 200;
            case BOTTOM:
                return 176;
            default:
                return 0;
        }
    }

    Color getTint(Direction face) const
    {
        unsigned char luminosity = getLuminosity(face);
        return { luminosity, luminosity, luminosity, 255 };
    }
    //// End of GETTERS ////



    //// DRAWING ////
    void drawFace(const Vector3 & position, BlockType adjacentBlock, Direction face) const
    {
        if (this->isSurfaceVisible(adjacentBlock)) // && this->block == SAND)
        {
            const Color & color = ColorTint(this->getColor(), this->getTint(face));
            const array<int, 4> & faceIndexes = faceIndexesMap.find(face)->second;
            DrawTriangle3D(position + this->verticesPosArr[faceIndexes[0]], position + this->verticesPosArr[faceIndexes[1]], position + this->verticesPosArr[faceIndexes[2]], color);
            DrawTriangle3D(position + this->verticesPosArr[faceIndexes[0]], position + this->verticesPosArr[faceIndexes[2]], position + this->verticesPosArr[faceIndexes[3]], color);

            if (this->block == WATER)
            {
                DrawTriangle3D(position + this->verticesPosArr[faceIndexes[0]], position + this->verticesPosArr[faceIndexes[3]], position + this->verticesPosArr[faceIndexes[2]], color);
                DrawTriangle3D(position + this->verticesPosArr[faceIndexes[0]], position + this->verticesPosArr[faceIndexes[2]], position + this->verticesPosArr[faceIndexes[1]], color);
            }

            // // Display the inside of the world mesh. Only useful for debugging.
            // for (int i = 0; i < 4; i++)
            //     DrawLine3D(position + this->verticesPosArr[faceIndexes[i]], position + this->verticesPosArr[faceIndexes[(i + 1)%4]], color);
            // DrawLine3D(position + this->verticesPosArr[faceIndexes[0]], position + this->verticesPosArr[faceIndexes[2]], color);
        }
    }

    void draw(const Vector3 position) const
    {
        DrawCube(position, 1.0f, 1.0f, 1.0f, this->getColor());
        this->drawLines(position);
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
    //// End of DRAWING ////
};