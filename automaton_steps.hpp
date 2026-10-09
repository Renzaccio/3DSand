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
        for (int i = 0; i < wlen; i++)
        {
            for (int j = 0; j < hlen; j++)
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

    static StackPartitions* safeGetStack(int x, int z, vector<vector<StackPartitions>>& stacks)
    {
        if (x < 0 || x >= static_cast<int>(stacks.size()))
            return nullptr;

        if (z < 0 || z >= static_cast<int>(stacks[x].size()))
            return nullptr;

        return &stacks[x][z];
    };
    
    static void sandCollapseStack(WorldMtx& mtx, vector<vector<StackPartitions>>& stacks, int wlen, int hlen)
    {
        // Si le tas de la heightmap a la partition [i,j][p] est > e_0,
        // si le sommet de la pile actuel soustrait au sommet de la pile d'un de ses voisins > e_1
        // simplement decaler le block au sommet de la pile actuel sur x ou/et z mais sur le meme y avec un simple +1
        // il tombera tout seul grace a sandFall   

        for (int i = 0; i < hlen; i++)
        {
            for(int j = 0; j < wlen; j++)
            {
                StackPartitions sp = stacks[i][j];

                for (int k = 0; k < sp.size(); k++)
                {
                    int idxBeginOfThisPartition = sp[k][0];
                    int sizeOfThisPartition = sp[k][1];

                    if (sizeOfThisPartition > 5)
                    {
                        if ((j+1) < wlen)
                        {
                            // On cherche le voisin dont le sommet actuel moins celui du voisin >= 5  
                            // Le sommet(i,j) = somme des (stacks[i][j].partition_k[0] + stacks[i][j].partition_k[1]) 

                            StackPartitions* lt = AutomatonSteps::safeGetStack(i-1,j-1, stacks); StackPartitions* mt = AutomatonSteps::safeGetStack(i-1, j, stacks); StackPartitions* rt = AutomatonSteps::safeGetStack(i-1, j+1, stacks);
                            StackPartitions* lm = AutomatonSteps::safeGetStack(i,j-1, stacks); /**mid */                                             StackPartitions* rm = AutomatonSteps::safeGetStack(i, j+1, stacks);
                            StackPartitions* lb = AutomatonSteps::safeGetStack(i+1,j-1, stacks); StackPartitions* mb = AutomatonSteps::safeGetStack(i+1, j, stacks); StackPartitions* rb = AutomatonSteps::safeGetStack(i+1, j+1, stacks);
                            
                            // array<StackPartitions*, 8> neighbors{{lt, lm, lb, mt, mb, rt, rm, rb}}; 

                            // for (int g = 0; g < 8; g++)
                            // {
                            //     if(neighbors[g] == nullptr) continue;

                            //     StackPartitions* curnei = neighbors[g];
                            //     //sp = safeGetStack(i,j,stacks)
                            // }

                            mtx[i][idxBeginOfThisPartition+(sizeOfThisPartition-1)][j] = AIR;
                            mtx[i][idxBeginOfThisPartition+(sizeOfThisPartition-1)][j+1] = SAND;
                            sp[k][1] -= 1; 
                        }
                    }
                }
            }
        }
    }
};