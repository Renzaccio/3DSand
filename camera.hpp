#pragma once

#include "include/raylib.h"
#include "include/raymath.h"
#include <cmath>

using namespace std;

class OrientatedCamera : public Camera3D
{
private:
    float yaw;
    float pitch;
    Vector3 direction;
    float rotationSpeed;
    float speed;

public:
    OrientatedCamera() : Camera3D()
    {
        this->position = (Vector3){ -20.0f, 20.0f, -20.0f };
        this->up = (Vector3){ 0.0f, 10.0f, 0.0f };
        this->fovy = 45.0f;
        this->projection = CAMERA_PERSPECTIVE;
        this->speed = 15;
        this->rotationSpeed = PI / 2;

        this->yaw = PI / 4;
        this->pitch = atan(- 1 / sqrt(2));

        this->updateDirection();
        this->updateTarget();
    }
    
    void moveCamera(int fps)
    {
        // Camera Rotation //

        float frameRotationSpeed = this->rotationSpeed / fps;

        if (IsKeyDown(KEY_UP))
        {
            this->pitch += frameRotationSpeed;
            if (this->pitch >   PI / 2.1f) this->pitch =   PI / 2.1f;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            this->pitch -= frameRotationSpeed;
            if (this->pitch < - PI / 2.1f) this->pitch = - PI / 2.1f;
        }
        if (IsKeyDown(KEY_LEFT))  this->yaw   += frameRotationSpeed;
        if (IsKeyDown(KEY_RIGHT)) this->yaw   -= frameRotationSpeed;

        this->updateDirection();
        
        // End of Camera Rotation //
        
        // Camera Translation //

        float frameSpeed = this->speed / fps;

        if (IsKeyDown(KEY_W)) { this->position = Vector3Add(this->position, Vector3Scale(this->direction, frameSpeed  )); }
        if (IsKeyDown(KEY_S)) { this->position = Vector3Add(this->position, Vector3Scale(this->direction, - frameSpeed)); }

        if (IsKeyDown(KEY_A))
        {
            this->position.x += (cos(yaw)) * frameSpeed;
            this->position.z -= (sin(yaw)) * frameSpeed;
        }
        if (IsKeyDown(KEY_D))
        {
            this->position.x -= (cos(yaw)) * frameSpeed;
            this->position.z += (sin(yaw)) * frameSpeed;
        }

        if (IsKeyDown(KEY_E)) { this->position.y += frameSpeed; }
        if (IsKeyDown(KEY_Q)) { this->position.y -= frameSpeed; }

        this->updateTarget();
        
        // End of Camera Translation //
    }

    void updateDirection()
    {
        this->direction = (Vector3)
        {
            cos(pitch) * sin(yaw),
            sin(pitch),
            cos(pitch) * cos(yaw)
        };
    }
    
    void updateTarget() { this->target = Vector3Add(this->position, this->direction); }
};