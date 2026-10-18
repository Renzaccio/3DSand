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
    float frameSpeed;

    const float defaultRotationSpeed = PI * 0.5f;
    float frameRotationSpeed;

    const float speedFactor = 3.0f;

    bool isInFirstPerson;
    float focusDistance;

public:

    OrientatedCamera() : Camera3D()
    {
        this->position = { -40.0f, 40.0f, -40.0f };
        this->up = { 0.0f, 1.0f, 0.0f };

        this->yaw = PI * 0.25f;
        this->pitch = atan(- sqrt(2.0f) * 0.5f);

        this->fovy = 45.0f;
        this->projection = CAMERA_PERSPECTIVE;

        this->isInFirstPerson = true;
        this->focusDistance = 100.0f;
    }

    bool getIsInFirstPerson() const { return isInFirstPerson; }

    void moveCamera(int fps)
    {
        fps = max(fps, 1); // Safety to prevent a division by 0 further in the code

        this->setFrameSpeed(fps);

        if (IsKeyPressed(KEY_F))
            this->isInFirstPerson = !this->isInFirstPerson;

        this->rotateCameraKeys();

        this->translateCameraKeys();
    }

    void setFrameSpeed(int fps)
    {
        this->frameSpeed = this->defaultSpeed / fps;
        this->frameRotationSpeed = this->defaultRotationSpeed / fps;

        switch (IsKeyDown(KEY_LEFT_SHIFT) - IsKeyDown(KEY_LEFT_CONTROL))
        {
            case -1 : // Slowed Movement
                this->frameSpeed /= this->speedFactor;
                // this->frameRotationSpeed /= this->speedFactor;
                break;
            case 1 : // Accelerated movement
                this->frameSpeed *= this->speedFactor;
                // this->frameRotationSpeed *= this->speedFactor;
                break;
        }
    }

    void rotateCameraKeys()
    {
        this->yaw   += (IsKeyDown(KEY_LEFT) - IsKeyDown(KEY_RIGHT)) * this->frameRotationSpeed;
        this->pitch += (IsKeyDown(KEY_UP)   - IsKeyDown(KEY_DOWN))  * this->frameRotationSpeed;

        if (abs(this->pitch) > PI * 0.49f) this->pitch = PI * 0.49f * (this->pitch > 0 ? 1 : -1); // Application of Pitch Bounds

        this->updateDirFromAngle();

        if (this->isInFirstPerson)
            this->updateTarFromPosAndDir(); // Camera rotates around its position
        else
            this->updatePosFromTarAndDir(); // Camera rotates around a focal points located in front of it
    }

    void translateCameraKeys()
    {
        Vector3 forwardDirection;
        Vector3 verticalDirection;

        if (this->isInFirstPerson) // The camera translation depends on its pitch
        {
            forwardDirection = this->direction;
            verticalDirection = this->up - this->direction * Vector3DotProduct(this->up, this->direction);
            verticalDirection /= Vector3Length(verticalDirection);
        }
        else // The camera translation doesn't depend on its pitch
        {
            forwardDirection = this->direction - this->up * Vector3DotProduct(this->direction, this->up);
            forwardDirection /= Vector3Length(forwardDirection);
            verticalDirection = this->up;
        }

        Vector3 lateralDirection = Vector3CrossProduct(verticalDirection, forwardDirection);

        this->position += ( // Position update
            (IsKeyDown(KEY_W) - IsKeyDown(KEY_S)) * forwardDirection +
            (IsKeyDown(KEY_A) - IsKeyDown(KEY_D)) * lateralDirection +
            (IsKeyDown(KEY_E) - IsKeyDown(KEY_Q)) * verticalDirection
        ) * this->frameSpeed;

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
        if (!this->isInFirstPerson) // Small 3D cross located at the point the camera rotates around
        {
            DrawLine3D(target - (Vec3I){ 1, 0, 0 }, target + (Vec3I){ 1, 0, 0 }, BLACK);
            DrawLine3D(target - (Vec3I){ 0, 1, 0 }, target + (Vec3I){ 0, 1, 0 }, BLACK);
            DrawLine3D(target - (Vec3I){ 0, 0, 1 }, target + (Vec3I){ 0, 0, 1 }, BLACK);
        }
    }
};