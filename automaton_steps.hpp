#pragma once

#include "blocks.hpp"
#include "include/raylib.h"
#include <iostream>
#include <vector>
#include <array>

using namespace std;

typedef vector<array<int, 2>> StackPartitions;
typedef vector<vector<vector<BlockType>>> WorldMtx;

class AutomatonSteps
{
private:
public:
    static void sandFall(WorldMtx& mtx, int wlen, int hlen)
    {
        for (int j = 0; j < hlen; j++)
        {
            for (int i = 0; i < wlen; i++)
            {
                for (int k = 0; k < wlen; k++)
                {
                    BlockType current_block = mtx[i][j][k];
                    Vector3 current_position = Vector3 { (float) i, (float) j, (float) k};

                    switch(current_block)
                    {
                        case SAND:
                            if (mtx[i][j-1][k] == AIR)
                            {
                                mtx[i][j-1][k] = SAND;
                                mtx[i][j][k] = AIR;
                            }

                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
};