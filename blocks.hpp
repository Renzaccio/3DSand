#pragma once

#include <vector>

using namespace std;

enum BlockType { AIR, STONE, DIRT, WOOD, SAND, BORDER, DEBUG };

struct Voxel
{
    BlockType block;
    bool canMove = true;
    bool isVisible = true;

    Voxel() : block{ AIR } {}
    Voxel(BlockType bl) : block{ bl } {}
};

typedef vector<vector<vector<Voxel>>> WorldMTX;