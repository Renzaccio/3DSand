#pragma once

#include "automaton_steps.hpp"
#include "include/raylib.h"
#include "blocks.hpp"
#include <array>
#include <vector>
#include <iostream>

using namespace std;

typedef vector<array<int, 2>> StackPartitions;
void printStackPartitions(const vector<vector<StackPartitions>>& sp, int hlen, int wlen)
{
    for (int i = 0; i < hlen; i++)
    {
        for (int j = 0; j < wlen; j++)
        {
            StackPartitions sp1 = sp[i][j];
            cout << "Sur " << i << ":" << j << " - il y a " << sp1.size() << " partitions de tas." << endl;

            for (int k = 0; k < sp1.size(); k++)
            {
                cout << "\t La " << k+1 << "-ieme partition commence a y=" << sp1[k][0] << " et il finit a y=" << (sp1[k][0]+sp1[k][1]) << ". hauteur=" << sp1[k][1] << endl;
            }
        }
        cout << endl;
    }
}

template<int WLEN, int HLEN>
class Integer3DSpace
{
private:
    vector<vector<vector<BlockType>>> mtx;
public: 
    Integer3DSpace() : mtx(WLEN, vector<vector<BlockType>>(HLEN, vector<BlockType>(WLEN, AIR))) {}

    void putBlockAt(BlockType bl, int x, int y, int z)
    {
        this->mtx[x][y][z] = bl;
    }

    void createOneBlockOnTop()
    {
        int randNum = rand()%(0-WLEN + 1) + 0;
        int randNum2 = rand()%(0-WLEN + 1) + 0;
        this->mtx[randNum][HLEN-1][randNum2] = SAND;
    }

    void step()
    {
        // Le sable tombe
        AutomatonSteps::sandFall(this->mtx, WLEN, HLEN);
        
        vector<vector<StackPartitions>> hm = this->getHeightMap();
        AutomatonSteps::sandCollapseStack(this->mtx, hm, WLEN, HLEN);
    }
    
    int area()
    {
        return WLEN*WLEN*HLEN;
    }

    int getHeightSize() const
    {
        return HLEN;
    }

    int getWidthSize() const
    {
        return WLEN;
    }

    int getDepthSize() const
    {
        return WLEN;
    }
    
    void createFlatFloor(int y, BlockType block)
    {
        if (y >= HLEN)
        {
            cout << "y est trop grand..." << endl;
            return;
        }

        for (int i = 0; i < WLEN; i++)
        {
            for (int j = 0; j < WLEN; j++)
            {
                this->mtx[i][y][j] = block;
            }
        }
    }

    BlockType getVoxelAt(unsigned int x, unsigned int y, unsigned int z) const
    {
        return this->mtx[x][y][z];
    }

    void drawWorld()
    {
        for (int i = 0; i < WLEN; i++)
        {
            for (int j = 0; j < HLEN; j++)
            {
                for (int k = 0; k < WLEN; k++)
                {
                    Vector3 position{(float) i,(float) j,(float) k};

                    switch (this->mtx[i][j][k])
                    {
                        case DIRT:
                            DrawCube(position, 1.0f, 1.0f, 1.0f, BROWN);
                            break;
                        case SAND:
                            DrawCube(position, 1.0f, 1.0f, 1.0f, YELLOW);
                            DrawCubeWires(position, 1.0f, 1.0f, 1.0f, BROWN);
                            break;
                        case DEBUG:
                            DrawCube(position, 1.0f, 1.0f, 1.0f, PURPLE);
                            DrawCubeWires(position, 1.0f, 1.0f, 1.0f, ORANGE);
                            break;
                        case AIR:
                            break;
                    }
                }
            }
        }
    }

    // Le retour est en 3D et non en 2D. Le pricipal
    // pb est de prevoir le moment ou on devra ajouter des objets statiques.
    // Supposons qu'on regarde la pile au dessus du voxel a la position {0,0,0}
    // Si un objet statique bloque la route a {0,5,0}, 
    // ca serait faux de considerer (getHeightMap())[0][0]=10 comme "a la cellule x,y, la pile de sable est de 10 blocks.".
    // On segmente donc les sous-piles des piles. Pour une pile sans obstacle,  (getHeightMap())[0][1]={{y,size}} ou y est le debut de la sous pile
    // et size la taille de la sous pile a partir de z. Si deux sous piles sont separer par un obstacle,
    //  (getHeightMap())[0][2]={{y=0,y+2}, {y+4,size}}, on voit tout de suite qu'un obstacle se trouve a x=0,y=y+3,z=0. 
    vector<vector<StackPartitions>> getHeightMap()
    {
        vector<vector<StackPartitions>> stacks_acc(HLEN, vector<StackPartitions>(WLEN));
        for (int i = 0; i < WLEN; i++)
        {
            for (int j = 0; j < WLEN; j++)
            {
                StackPartitions stack_acc;
                array<int, 2> tmp{{-1,0}}; // {y_start,size_stack} de la pile a la case x,y,z

                // on check unitairement chaque pile
                for (int k = 0; k < HLEN; k++)
                {
                    BlockType current_voxel = this->mtx[i][k][j];

                    if (current_voxel == SAND)
                    {    
                        if (tmp[0] == -1)
                        {
                            tmp[0] = k;
                        }

                        tmp[1] += 1;
                    } else {
                        if (tmp[0] != -1)
                        {
                            stack_acc.push_back(tmp);
                            tmp[0] = -1; tmp[1] = 0; // reset
                        }
                    }

                    if ((k == (HLEN-1)) && (tmp[0] != -1)) 
                    {
                        stack_acc.push_back(tmp);
                    }
                }

                stacks_acc[i][j] = stack_acc;
            }
        }
        //printStackPartitions(stacks_acc, HLEN, WLEN);
        return stacks_acc;
    }
    
    void printWorld()
    {
        for (int i = 0; i < WLEN; i++)
        {
            for (int j = 0; j < HLEN; j++)
            {
                for (int k = 0; k < WLEN; k++)
                {
                    cout << this->mtx[i][j][k]; 
                }
                cout << endl;
            }
            cout << endl;
        }
    }
};
