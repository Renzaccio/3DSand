#pragma once

#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include "../raylib/include/raylib.h"
#include "counter.hpp"

using namespace std; 

struct RPressButton : Rectangle
{
private:

    bool             isVisible = true;
    Color            color{ 255,255,255,255 };
    string           text = "";
    int              textSize;
    Color            textColor{ 0,0,0,255 };
    function<void(RPressButton&)> callback; // a acces a l'object
    string           payload = "unknown";

public:

    RPressButton(float x, float y, float width, float height, Color col, function<void(RPressButton&)> cb, string txt, int txtSize, Color txtCol) :
        Rectangle{ x,y,width,height }, color{ col }, callback{ cb }, text(txt), textSize{ txtSize }, textColor{ txtCol } {}

    void draw()               const { this->draw(this->x, this->y); }
    void draw(int yY)         const { this->draw(this->x, yY); }
    void draw(int xX, int yY) const
    {
        DrawRectangle(xX, yY, this->width, this->height, this->color);
        DrawText(this->text.c_str(), xX, yY, this->textSize, this->textColor);
    }

    string getPayload() { return this->payload; }
    void setPayload(string pa) { this->payload = pa; }

    void setVisibility(bool vb)   { this->isVisible = vb; }
    bool buttonIsVisible() const  { return this->isVisible; }

    void executeIfPressed() 
    {
        if (CheckCollisionPointRec(GetMousePosition(), *this))
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                this->callback(*this);
    }
};

// group of button. nothing special but we can
// execute action for all buttons.
struct RPressButtonLogicalGroup
{
private:

    vector<RPressButton> rbtns;

public:

    RPressButtonLogicalGroup() {}
    RPressButtonLogicalGroup(vector<RPressButton> rbs) :  rbtns{std::move(rbs)} {}

    void push(RPressButton btn)
    {
        this->rbtns.push_back(btn);
    }

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
            if (rb.buttonIsVisible())
                rb.executeIfPressed();
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
                rb.setVisibility(true);  
            }
        );
    }

    void showOne(int idx) 
    { 
        if (isExist(idx)) 
            this->rbtns[idx].setVisibility(true); 
    }
}; 

struct DockActionSupport {
    string originMenu;
    RPressButtonLogicalGroup rbtns;
    Counter<80> beginXActions;
};

typedef unordered_map<string, DockActionSupport> DockActions;
typedef unordered_map<string, RPressButton> DockMenu;

struct Dock
{
    DockMenu menu;
    DockActions actions;
    string currentMenu = "";
    Counter<50> beginYMenu; 
    bool isVisible = true;

    void addSectionMenu(string menuKey, string menuName)
    {
        if (!this->menu.contains(menuKey))
        {
            RPressButton rbtn { (float) (GetRenderWidth()-100), (float) beginYMenu(), 70.0, 30.0, RED, 
                [this, menuKey](RPressButton& _) {
                    this->hideAllActions(); // Hide all actions menus
                    this->selectSectionMenu(menuKey);

                    for (auto& acts : this->actions) // show logical group that respect current key
                    {
                        // cout << acts.second.originMenu << endl;
                        if (acts.second.originMenu == menuKey)
                        {
                            acts.second.rbtns.showAll();
                            break;
                        }
                    }
                }, 
            menuKey, 15, BLACK };

            this->menu.insert_or_assign(menuKey, rbtn);
        }
    }

    void selectSectionMenu(string menuKey)
    {
        if (this->menu.contains(menuKey))
            this->currentMenu = menuKey;
    }

    void addAction(string menuKey, string actKey, string actName, function<void()> cb)
    {   
        if (this->actions.contains(actKey))
        {            
            RPressButton rbtn { (float) this->actions[actKey].beginXActions(), (float) (GetRenderHeight()-100), 70.0, 70.0, RED, 
                [actKey, cb](RPressButton& rpb) 
                { 
                    if (rpb.getPayload() == actKey)
                        cb();
                }
            , actName, 15, BLACK };

            rbtn.setVisibility(false);
            rbtn.setPayload(actKey);

            this->actions[actKey].rbtns.push(rbtn);
        } else
        { 
            RPressButton rbtn { (float) (GetRenderWidth()/3), (float) (GetRenderHeight()-100), 70.0, 70.0, RED, 
                [actKey, cb](RPressButton& rpb) 
                { 
                    if (rpb.getPayload() == actKey)
                        cb();
                }
            , actName, 15, BLACK };

            rbtn.setVisibility(false);
            rbtn.setPayload(actKey);

            this->actions.insert_or_assign(
                actKey, 
                DockActionSupport { 
                    menuKey, 
                    RPressButtonLogicalGroup { 
                        vector<RPressButton>{ {rbtn} } 
                    }, 
                    Counter<80>{ (GetRenderWidth()/3)+80 } });
        }
    }

    void hideAllActions() {
        for (auto& acts : this->actions)
            acts.second.rbtns.hideAll();
    }

    void draw()
    {
        for (auto& sectionMenu : this->menu)
            sectionMenu.second.draw();

        for (auto& acts : this->actions)
            if (this->currentMenu == acts.second.originMenu)
                acts.second.rbtns.drawAll();
    }

    void pressDetection()
    {
        for (auto& sectionMenu : this->menu)
            sectionMenu.second.executeIfPressed();

        for (auto& acts : this->actions)
            if (this->currentMenu == acts.second.originMenu)
                acts.second.rbtns.pressDetection();
    }
};

Dock buildMenu()
{
    Dock dock;
    
    dock.addSectionMenu("tools", "Tools");
    dock.addSectionMenu("blocks", "Blocks");
    dock.addSectionMenu("settings", "Settings");

    dock.addAction("tools", "test_1", "Test 1", []() {cout << "--- DEBUG 00x00" << endl;});
    dock.addAction("tools", "test_1", "Test 2", []() {cout << "--- DEBUG 00x00" << endl;});
    dock.addAction("tools", "test_1", "Test 3", []() {cout << "--- DEBUG 00x00" << endl;});

    dock.addAction("blocks", "test_2", "Test 4", []() {cout << "--- DEBUG 00x01" << endl;});
    dock.addAction("blocks", "test_2", "Test 5", []() {cout << "--- DEBUG 00x01" << endl;});
    dock.addAction("blocks", "test_2", "Test 6", []() {cout << "--- DEBUG 00x01" << endl;});

    dock.addAction("settings", "test_3", "Test 7", []() {cout << "--- DEBUG 00x02" << endl;});
    dock.addAction("settings", "test_3", "Test 8", []() {cout << "--- DEBUG 00x02" << endl;});
    dock.addAction("settings", "test_3", "Test 9", []() {cout << "--- DEBUG 00x02" << endl;});

    return dock;
}