#pragma once

#include "include/raylib.h"

using namespace std;

class OrientatedCamera : public Camera3D
{
private:
    double yaw;
    double pitch;
public:

    OrientatedCamera() : Camera3D()
    {
        this->position = (Vector3){ 10.0f, 10.0f, -10.0f };
        this->target = (Vector3){ 0.0f, 0.0f, 0.0f }; 
        this->up = (Vector3){ 0.0f, 10.0f, 0.0f };
        this->fovy = 45.0f; 
        this->projection = CAMERA_PERSPECTIVE;
    }
    
    void moveCamera()
    {
        if (IsKeyDown(KEY_RIGHT))
        {
            this->position.x--;
            this->target.x--;
        }

        if (IsKeyDown(KEY_LEFT))
        {
            this->position.x++;
            this->target.x++;
        }

        if (IsKeyDown(KEY_UP))
        {
            this->position.z++;
            this->target.z++;
        }

        if (IsKeyDown(KEY_DOWN))
        {
            this->position.z--;
            this->target.z--;
        }

        if (IsKeyDown(KEY_KP_8))
        {
            this->position.y++;
            this->target.y++;
        }

        if (IsKeyDown(KEY_KP_2))
        {
            this->position.y--;
            this->target.y--;
        }
    }
};