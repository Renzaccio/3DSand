#pragma once

#include <string>
#include <functional>
#include "include/raylib.h"

using namespace std; 

struct RPressButton : Rectangle
{
private:

    Color            color{ 255,255,255,255 };
    string           text = "";
    int              textSize;
    Color            textColor{ 0,0,0,255 };
    function<void()> callback;

public:

    RPressButton(float x, float y, float width, float height, Color col, function<void()> cb, string txt, int txtSize, Color txtCol) :
        Rectangle{ x,y,width,height }, color{ col }, callback{ cb }, text(txt), textSize{ txtSize }, textColor{ txtCol } {}


    void draw()               const { this->draw(this->x, this->y); }
    void draw(int yY)         const { this->draw(this->x, yY); }
    void draw(int xX, int yY) const
    {
        DrawRectangle(xX, yY, this->width, this->height, this->color);
        DrawText(this->text.c_str(), xX, yY, this->textSize, this->textColor);
    }


    void executeIfPressed() const
    {
        if (CheckCollisionPointRec(GetMousePosition(), *this))
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                this->callback();
    }
};