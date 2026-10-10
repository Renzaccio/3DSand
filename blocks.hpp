#pragma once

#include <vector>
#include <array>

using namespace std; 

enum BlockType { AIR, SAND, DIRT, BORDER, DEBUG, STONE, WOOD };

struct Voxel
{
    BlockType block;
    Voxel(BlockType bl) : block{bl} {}
};

typedef vector<vector<vector<Voxel>>> WorldMTX; 

void placeChair3x4x3(WorldMTX& atmtx, array<int, 3> origin)
{
    int atx = origin[0];
    int aty = origin[1];
    int atz = origin[2];

    //pieds
    atmtx[atx][aty][atz] = Voxel{WOOD};
    atmtx[atx][aty][atz+2] = Voxel{WOOD};
    atmtx[atx+2][aty][atz] = Voxel{WOOD};
    atmtx[atx+2][aty][atz+2] = Voxel{WOOD};

    //plateau
    for (int x = 0; x < 3; x++)
    {
        for (int z = 0; z < 3; z++)
        {
            atmtx[atx+x][aty+1][atz+z] = Voxel{WOOD};
        }
    }

    //dossier
    for (int y = 2; y <= 3; y++)
    {
        for (int x = 0; x < 3; x++)
        {
            atmtx[atx+x][aty+y][atz+2] = Voxel{WOOD};
        }
    }
}

void placeTable5x5x5(WorldMTX& atmtx, array<int, 3> origin)
{
    int atx = origin[0];
    int aty = origin[1];
    int atz = origin[2];

    //pieds
    for (int y = 0; y < 3; y++)
    {
        atmtx[atx][aty+y][atz] = Voxel{STONE};
        atmtx[atx][aty+y][atz+4] = Voxel{STONE};
        atmtx[atx+4][aty+y][atz] = Voxel{STONE};
        atmtx[atx+4][aty+y][atz+4] = Voxel{STONE};
    }

    //plateau
    for (int x = 0; x < 5; x++)
    {
        for (int z = 0; z < 5; z++)
        {
            atmtx[atx+x][aty+3][atz+z] = Voxel{WOOD};
        }
    }
}