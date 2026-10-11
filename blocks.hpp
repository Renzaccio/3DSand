#pragma once

#include <vector>
#include "i3dvector.hpp"

using namespace std; 

enum BlockType { AIR, SAND, DIRT, BORDER, DEBUG, STONE, WOOD };

struct Voxel
{
    BlockType block;
    Voxel(BlockType bl) : block{bl} {}
};

typedef vector<vector<vector<Voxel>>> WorldMTX;

void placeChair3x4x3(WorldMTX & atmtx, const Vec3I & origin)
{
    //pieds
    atmtx[origin.x  ][origin.y][origin.z  ] = Voxel{WOOD};
    atmtx[origin.x  ][origin.y][origin.z+2] = Voxel{WOOD};
    atmtx[origin.x+2][origin.y][origin.z  ] = Voxel{WOOD};
    atmtx[origin.x+2][origin.y][origin.z+2] = Voxel{WOOD};

    //plateau
    for (int x = 0; x < 3; x++)
    {
        for (int z = 0; z < 3; z++)
        {
            atmtx[origin.x+x][origin.y+1][origin.z+z] = Voxel{WOOD};
        }
    }

    //dossier
    for (int y = 2; y <= 3; y++)
    {
        for (int x = 0; x < 3; x++)
        {
            atmtx[origin.x+x][origin.y+y][origin.z+2] = Voxel{WOOD};
        }
    }
}

void placeTable5x5x5(WorldMTX & atmtx, const Vec3I & origin)
{
    //pieds
    for (int y = 0; y < 3; y++)
    {
    atmtx[origin.x  ][origin.y+y][origin.z  ] = Voxel{STONE};
    atmtx[origin.x  ][origin.y+y][origin.z+4] = Voxel{STONE};
    atmtx[origin.x+4][origin.y+y][origin.z  ] = Voxel{STONE};
    atmtx[origin.x+4][origin.y+y][origin.z+4] = Voxel{STONE};
    }

    //plateau
    for (int x = 0; x < 5; x++)
    {
        for (int z = 0; z < 5; z++)
        {
            atmtx[origin.x+x][origin.y+3][origin.z+z] = Voxel{WOOD};
        }
    }
}