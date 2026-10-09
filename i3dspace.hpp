#pragma once

#include "automaton_steps.hpp"
#include "include/raylib.h"
#include "blocks.hpp"
// #include <array>
#include <vector>
#include <iostream>

using namespace std;

template<int WLEN, int HLEN>
class Integer3DSpace
{
private:
    array<array<array<BlockType, WLEN>, HLEN>, WLEN> mtx;
public: 
    Integer3DSpace() : mtx(WLEN, vector<vector<BlockType>>(HLEN, vector<BlockType>(WLEN, AIR))) {}

    const array<array<BlockType, WLEN>, HLEN> & operator [] (int i) const { return mtx[i]; }
          array<array<BlockType, WLEN>, HLEN> & operator [] (int i)       { return mtx[i]; }

    BlockType block(int x, int y, int z) const
    {
        if(x < 0 || WLEN <= x || y < 0 || HLEN <= y || z < 0 || WLEN <= z) return BORDER;
        return this->mtx[x][y][z];
    }

    BlockType & block(int x, int y, int z)
    {
        return this->mtx[x][y][z];
    }

    void createOneBlockOnTop()
    {
        int randX = rand()%WLEN;
        int randY = rand()%WLEN;
        this->mtx[randX][HLEN-1][randY] = SAND;
    }

    void step()
    {
        // Le sable tombe
        AutomatonSteps::sandFall(this->mtx, WLEN, HLEN);
    }
    
    int volume()
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

    void drawWorld() const
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
