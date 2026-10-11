#pragma once
#include "include/raylib.h"
#include <string>
#include <functional>

using namespace std; 

struct RPressButton : Rectangle
{
    Color color{1,1,1,255};
    string text = "";
    int textSize;
    Color textColor{1,1,1,255};
    function<void()> callback;
    

    RPressButton(float x, float y, float width, float height, Color co, function<void()> cb, string txt, int txtSize, Color txtCo) 
        : Rectangle{x,y,width,height}, callback(cb), color{co}, text(txt), textSize(txtSize), textColor{txtCo} {}

    void draw()
    {
        DrawRectangle(this->x, this->y, this->width, this->height, this->color);
        DrawText(text.c_str(), this->x, this->y, this->textSize, this->textColor);
    }

    void operator()()
    {
        callback();
    }
};