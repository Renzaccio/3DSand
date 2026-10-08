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

        this->speed = 15.0f;
        this->rotationSpeed = PI * 0.5f;

        this->yaw = PI * 0.25f;
        this->pitch = atan(- sqrt(2.0f) * 0.5f);

        this->updateDirection();
        this->updateTarget();
    }
    
    void moveCamera(int fps)
    {
        rotateCameraKeys(fps);

        translateCameraKeys(fps);
    }

    void rotateCameraKeys(int fps)
    {
        float frameRotationSpeed = this->rotationSpeed / fps;

        if (IsKeyDown(KEY_UP))
        {
            this->pitch += frameRotationSpeed;
            if (this->pitch >   PI * 0.45f) this->pitch =   PI * 0.45f;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            this->pitch -= frameRotationSpeed;
            if (this->pitch < - PI * 0.45f) this->pitch = - PI * 0.45f;
        }
        
        if (IsKeyDown(KEY_LEFT))  this->yaw += frameRotationSpeed;
        if (IsKeyDown(KEY_RIGHT)) this->yaw -= frameRotationSpeed;

        this->updateDirection();
    }

    void translateCameraKeys(int fps)
    {
        float frameSpeed = this->speed / fps;

        if (IsKeyDown(KEY_W)) { this->position = this->position + this->direction * frameSpeed; }
        if (IsKeyDown(KEY_S)) { this->position = this->position - this->direction * frameSpeed; }

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
    
    void updateTarget() { this->target = this->position + this->direction; }
};