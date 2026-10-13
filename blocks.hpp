#pragma once

#include <iostream>
#include "include/raylib.h"

using namespace std;

enum BlockType { BORDER, DEBUG, STONE, WOOD, DIRT, SAND, AIR };

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
    bool canMove   = true;
    bool isVisible = true;


    Voxel() : block{ AIR } {}
    Voxel(BlockType bl) : block{ bl } {}


    void draw(const Vector3 position) const
    {
        switch (this->block)
        {
            case DEBUG:
                DrawCube(position, 1.0f, 1.0f, 1.0f, PURPLE);
                DrawCubeWires(position, 1.0f, 1.0f, 1.0f, ORANGE);
                break;
            case STONE:
                DrawCube(position, 1.0f, 1.0f, 1.0f, GRAY);
                break;
            case WOOD:
                DrawCube(position, 1.0f, 1.0f, 1.0f, BROWN);
                DrawCubeWires(position, 1.0f, 1.0f, 1.0f, BLACK);
                break;
            case DIRT:
                DrawCube(position, 1.0f, 1.0f, 1.0f, BROWN);
                break;
            case SAND:
                DrawCube(position, 1.0f, 1.0f, 1.0f, YELLOW);
                DrawCubeWires(position, 1.0f, 1.0f, 1.0f, BROWN);
                break;
            default:
                break;
        }
    }
};