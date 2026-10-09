#pragma once

#include "blocks.hpp"
#include "include/raylib.h"
#include "i3dspace.hpp"
// #include <iostream>
#include <vector>
#include <array>

using namespace std;

typedef vector<array<int, 2>> StackPartitions;

template<int WLEN, int HLEN>
class AutomatonSteps
{
private:
public:
    static void sandFall(Integer3DSpace<WLEN, HLEN> & sworld, int wlen, int hlen)
    {
        for (int j = 0; j < hlen; j++)
        {
            for (int i = 0; i < wlen; i++)
            {
                for (int k = 0; k < wlen; k++)
                {
                    BlockType & current_block = sworld.block(i, j, k);
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