#pragma once

#include <array>

using namespace std;

enum BlockType { AIR, SAND, DIRT, BORDER, DEBUG };

struct Voxel
{
    BlockType block;
    array<int, 3> coords;

    Voxel(BlockType bl) : block{bl} {}
    Voxel(BlockType bl, array<int, 3> co) : Voxel(bl) 
    {
        this->coords = co;
    }
};