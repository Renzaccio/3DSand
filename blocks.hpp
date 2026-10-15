#pragma once

#include <iostream>
#include "include/raylib.h"
#include "include/raymath.h"

using namespace std;

enum BlockType { BORDER, DEBUG, STONE, WOOD, DIRT, SAND, AIR };

enum BlockClass { SOLID, LIQUID, GAS };

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

    void drawZ(const Vector3 & position, BlockType adjacentBlock) const
    {
        if (this->isSurfaceVisible(adjacentBlock))
        {
            Color color = this->getColor();
            DrawTriangle3D(position + Vector3{ 0.0f, 1.0f, 0.0f }, position + Vector3{ 0.0f, 1.0f, 1.0f }, position + Vector3{ 1.0f, 1.0f, 0.0f }, color);
            DrawTriangle3D(position + Vector3{ 1.0f, 1.0f, 0.0f }, position + Vector3{ 0.0f, 1.0f, 1.0f }, position + Vector3{ 1.0f, 1.0f, 1.0f }, color);
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