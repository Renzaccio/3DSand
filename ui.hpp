#pragma once

#include <algorithm>
#include <string>
#include <functional>
#include "include/raylib.h"
#include <vector>

using namespace std; 

struct RPressButton : Rectangle
{
private:
    bool             isVisible = true;
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

    void setVisibility(bool vb)   { this->isVisible = vb; }
    bool buttonIsVisible() const  { return this->isVisible; }

    void executeIfPressed() const
    {
        if (CheckCollisionPointRec(GetMousePosition(), *this))
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                this->callback();
    }
};

// group of button. nothing special but we can
// execute action for all buttons.
struct RPressButtonLogicalGroup
{
private:
    vector<RPressButton> rbtns;
public:
    RPressButtonLogicalGroup(vector<RPressButton> rbs) :  rbtns{rbs} {}

    void drawAll()
    {
        for (RPressButton& rb : rbtns)
        {
            if (rb.buttonIsVisible())
            {
                rb.draw();
            }
        }
    }

    void pressDetection()
    {
        for (RPressButton& rb : rbtns)
        {
            if (rb.buttonIsVisible())
            {
                rb.executeIfPressed();
            }
        }
    }

    bool isExist(int idx) { return (idx >= 0) && (idx < this->rbtns.size()); }

    void hideOne(int idx) 
    { 
        if (isExist(idx)) 
            this->rbtns[idx].setVisibility(false); 
    }

    void hideAll() 
    { 
        for_each(
            this->rbtns.begin(), 
            this->rbtns.end(), 
            [](RPressButton& rb) { 
                rb.setVisibility(false);  
            }
        );
    }

    void showAll() 
    { 
        for_each(
            this->rbtns.begin(), 
            this->rbtns.end(), 
            [](RPressButton& rb) { 
                rb.setVisibility(false);  
            }
        );
    }

    void showOne(int idx) 
    { 
        if (isExist(idx)) 
            this->rbtns[idx].setVisibility(true); 
    }
}; 
