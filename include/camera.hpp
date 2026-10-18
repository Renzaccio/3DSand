#pragma once

#include "../raylib/include/raylib.h"
#include "../raylib/include/raymath.h"
#include "i3dvector.hpp"

struct OrientatedCamera : public Camera3D
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

    bool isInFirstPerson;
    const float focusDistance = 100.0f;

public:
    OrientatedCamera() : Camera3D()
    {
        this->position = { -40.0f, 40.0f, -40.0f };
        this->up = { 0.0f, 1.0f, 0.0f };

        this->fovy = 45.0f;
        this->projection = CAMERA_PERSPECTIVE;

        this->yaw = PI * 0.25f;
        this->pitch = atan(- sqrt(2.0f) * 0.5f);

        this->isInFirstPerson = true;
    }

    bool getIsInFirstPerson() const { return isInFirstPerson; }

    void moveCamera(int fps)
    {
        this->setSpeedMode();

        if (IsKeyPressed(KEY_F))
            this->isInFirstPerson = !this->isInFirstPerson;

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
                // this->rotationSpeed = this->defaultRotationSpeed / this->speedFactor;
                break;
            case 0 :
                this->speed = this->defaultSpeed;
                // this->rotationSpeed = this->defaultRotationSpeed;
                break;
            case 1 :
                this->speed = this->defaultSpeed * this->speedFactor;
                // this->rotationSpeed = this->defaultRotationSpeed * this->speedFactor;
                break;
        }

        this->rotationSpeed = defaultRotationSpeed;
    }

    void rotateCameraKeys(int fps)
    {
        float frameRotationSpeed = this->rotationSpeed / fps * (this->isInFirstPerson ? 1 : -1);

        if (IsKeyDown(KEY_LEFT))  this->yaw   += frameRotationSpeed;
        if (IsKeyDown(KEY_RIGHT)) this->yaw   -= frameRotationSpeed;

        if (IsKeyDown(KEY_UP))    this->pitch += frameRotationSpeed;
        if (IsKeyDown(KEY_DOWN))  this->pitch -= frameRotationSpeed;

        if (abs(this->pitch) > PI * 0.49f) this->pitch = PI * 0.49f * (this->pitch > 0 ? 1 : -1);

        this->updateDirFromAngle();

        if (this->isInFirstPerson)
            this->updateTarFromPosAndDir();
        else
            this->updatePosFromTarAndDir();
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

        this->updateTarFromPosAndDir();
    }

    void updateDirFromAngle()
    {
        this->direction.x = cos(this->pitch) * sin(this->yaw);
        this->direction.y = sin(this->pitch);
        this->direction.z = cos(this->pitch) * cos(this->yaw);
    }
    void updateDirFromTarAndPos() { this->direction = Vector3Normalize(this->target - this->position); }

    void updateTarFromPosAndDir() { this->target   = this->position + this->direction * this->focusDistance; }
    void updatePosFromTarAndDir() { this->position = this->target   - this->direction * this->focusDistance; }

    void drawInfo() const
    {
        if (!this->isInFirstPerson)
        {
            DrawLine3D(target - (Vec3I){ 1, 0, 0 }, target + (Vec3I){ 1, 0, 0 }, BLACK);
            DrawLine3D(target - (Vec3I){ 0, 1, 0 }, target + (Vec3I){ 0, 1, 0 }, BLACK);
            DrawLine3D(target - (Vec3I){ 0, 0, 1 }, target + (Vec3I){ 0, 0, 1 }, BLACK);
        }
    }
};