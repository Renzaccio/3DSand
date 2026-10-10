#pragma once

#include "include/raylib.h"
#include "include/raymath.h"
#include <cmath>
// #include <string>
// #include <iostream>

using namespace std;


class OrientatedCamera : public Camera3D
{
private:
    Vector3 direction;

    float yaw;
    float pitch;

    const float defaultSpeed = 10.0f;
    float speed;

    const float defaultRotationSpeed = PI * 0.5f;
    float rotationSpeed;

    const float speedFactor = 3.0f;

    bool isFocused;
    const float focusDistance = 10.0f;

public:
    OrientatedCamera() : Camera3D()
    {
        this->position = { -40.0f, 40.0f, -40.0f };
        this->up = { 0.0f, 1.0f, 0.0f };

        this->fovy = 45.0f;
        this->projection = CAMERA_PERSPECTIVE;

        this->yaw = PI * 0.25f;
        this->pitch = atan(- sqrt(2.0f) * 0.5f);

        this->isFocused = false;
    }

    void moveCamera(int fps)
    {
        this->setSpeedMode();

        // if (IsKeyPressed(KEY_F))
            // this->isFocused = !this->isFocused;

        if (!isFocused)
            this->rotateCameraKeys(fps);

        this->translateCameraKeys(fps);
    }

    void setSpeedMode()
    {
        int speedMode = IsKeyDown(KEY_LEFT_SHIFT) - IsKeyDown(KEY_LEFT_CONTROL);

        switch (speedMode)
        {
            case -1 :
                this->speed = this->defaultSpeed / this->speedFactor;
                this->rotationSpeed = this->defaultRotationSpeed / this->speedFactor;
                break;
            case 0 :
                this->speed = this->defaultSpeed;
                this->rotationSpeed = this->defaultRotationSpeed;
                break;
            case 1 :
                this->speed = this->defaultSpeed * this->speedFactor;
                this->rotationSpeed = this->defaultRotationSpeed * this->speedFactor;
                break;
        }
    }

    void rotateCameraKeys(int fps)
    {
        float frameRotationSpeed = this->rotationSpeed / fps;

        if (IsKeyDown(KEY_UP))
        {
            this->pitch += frameRotationSpeed;
            if (this->pitch >   PI * 0.49f) this->pitch =   PI * 0.49f;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            this->pitch -= frameRotationSpeed;
            if (this->pitch < - PI * 0.49f) this->pitch = - PI * 0.49f;
        }

        if (IsKeyDown(KEY_LEFT))  this->yaw += frameRotationSpeed;
        if (IsKeyDown(KEY_RIGHT)) this->yaw -= frameRotationSpeed;

        this->updateDirFromAngle();
    }

    void translateCameraKeys(int fps)
    {
        float frameSpeed = this->speed / fps;

        Vector3 verticalDirection = this->up - this->direction * Vector3DotProduct(this->up, this->direction);
        verticalDirection /= Vector3Length(verticalDirection);

        Vector3 horizontalDirection = Vector3CrossProduct(verticalDirection, this->direction);

        if (IsKeyDown(KEY_W)) this->position += frameSpeed * this->direction;
        if (IsKeyDown(KEY_S)) this->position -= frameSpeed * this->direction;

        if (IsKeyDown(KEY_A)) this->position += frameSpeed * horizontalDirection;
        if (IsKeyDown(KEY_D)) this->position -= frameSpeed * horizontalDirection;

        if (IsKeyDown(KEY_E)) this->position += frameSpeed * verticalDirection;
        if (IsKeyDown(KEY_Q)) this->position -= frameSpeed * verticalDirection;

        if (isFocused)
            this->updateDirFromTar();
        else
            this->updateTarFromDir();
    }

    void updateDirFromAngle()
    {
        this->direction.x = cos(this->pitch) * sin(this->yaw);
        this->direction.y = sin(this->pitch);
        this->direction.z = cos(this->pitch) * cos(this->yaw);
    }

    void updateTarFromDir() { this->target = this->position + this->direction * this->focusDistance; }

    void updateDirFromTar() { this->direction = Vector3Normalize(this->target - this->position); }
};