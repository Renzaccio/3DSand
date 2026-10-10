#pragma once

#include "i3dvector.hpp"

using namespace std;

enum BlockType { AIR, SAND, DIRT, BORDER, DEBUG };

struct Voxel
{
    BlockType block;
    Vec3I coords;

    Voxel(BlockType bl) : block{bl} {}
    Voxel(BlockType bl, Vec3I co) : Voxel(bl) 
    {
        this->coords = co;
    }
};