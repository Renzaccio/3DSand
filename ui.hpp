#pragma once

#include <string>
#include <functional>
#include "include/raylib.h"

using namespace std; 

struct RPressButton : Rectangle
{
    Color color{ 255,255,255,255 };
    string text = "";
    int textSize;
    Color textColor{ 0,0,0,255 };
    function<void()> callback;
    

    RPressButton(float x, float y, float width, float height, Color co, function<void()> cb, string txt, int txtSize, Color txtCo) :
        Rectangle{ x,y,width,height }, callback(cb), color{ co }, text(txt), textSize(txtSize), textColor{ txtCo } {}

    void draw()
    {
        draw(this->x, this->y);
    }

    void draw(int xX, int yY)
    {
        DrawRectangle(xX, yY, this->width, this->height, this->color);
        DrawText(text.c_str(), xX, yY, this->textSize, this->textColor);
    }
    
    void draw(int yY) {
        draw(this->x, yY);
    }

    void executeIfPressed()
    {
        if (CheckCollisionPointRec(GetMousePosition(), *this))
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                this->callback();
    }
};