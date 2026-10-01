#pragma once

#include "include/raylib.h"
#include "blocks.hpp"
#include <vector>
#include <iostream>

using namespace std;

template<int WLEN, int HLEN>
class Integer3DSpace
{
private:
    vector<vector<vector<BlockType>>> mtx;
public: 
    Integer3DSpace() : mtx(WLEN, vector<vector<BlockType>>(HLEN, vector<BlockType>(WLEN, AIR))) {}

    void createOneBlockOnTop()
    {
        int randNum = rand()%(0-WLEN + 1) + 0;
        int randNum2 = rand()%(0-WLEN + 1) + 0;
        this->mtx[randNum][HLEN-1][randNum2] = SAND;
    }

    void step()
    {
        for (int i = 0; i < WLEN; i++)
        {
            for (int j = 0; j < HLEN; j++)
            {
                for (int k = 0; k < WLEN; k++)
                {
                    BlockType current_block = this->mtx[i][j][k];
                    Vector3 current_position = Vector3 { (float) i, (float) j, (float) k};

                    switch(current_block)
                    {
                        case SAND:
                            if (this->mtx[i][j-1][k] == AIR)
                            {
                                this->mtx[i][j-1][k] = SAND;
                                this->mtx[i][j][k] = AIR;
                            }

                            break;
                        default:
                            break;
                    }
                }
            }
        }
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
                        case AIR:
                            break;
                    }
                }
            }
        }
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
