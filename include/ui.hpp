#pragma once

#include <algorithm>
#include <functional>
#include "counter.hpp"
#include "i3dspace.hpp"

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
        DrawRectangle(xX, yY, this->width+2, this->height+2, BLACK);
        DrawRectangle(xX, yY, this->width, this->height, this->color);

        int txtRealWidth = MeasureText(this->text.c_str(), this->textSize);
        int txtHeight = this->textSize;

        int posX = xX + ((this->width - txtRealWidth)/2);
        int posY = yY + ((this->height - txtHeight)/2);

        DrawText(this->text.c_str(), posX, posY, this->textSize, this->textColor);
    }

    string getPayload() { return this->payload; }
    void setPayload(string pa) { this->payload = pa; }

    void setBackgroundColor(Color co)     { this->color = co;}
    void setTextColor(Color co)     { this->textColor = co;}

    void setVisibility(bool vb)   { this->isVisible = vb; }
    bool buttonIsVisible() const  { return this->isVisible; }

    bool executeIfPressed()
    {
        if (CheckCollisionPointRec(GetMousePosition(), *this))
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                this->callback(*this);
                return true;
            }
        }

        return false;
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

    string pressDetection()
    {
        string lastexec = "unknown";

        for (RPressButton& rb : rbtns)
        {
            if (rb.buttonIsVisible())
            {
                if (rb.executeIfPressed())
                {
                    lastexec = rb.getPayload();
                }
            }
        }

        return lastexec;
    }

    bool isExist(int idx) { return (idx >= 0) && (idx < this->rbtns.size()); }

    void updateColorsByPayload(Color bgCo, Color txtCo, string payload)
    {
        for_each(
            this->rbtns.begin(),
            this->rbtns.end(),
            [payload, bgCo, txtCo](RPressButton& rb) {
                if (rb.getPayload() == payload)
                {
                    rb.setBackgroundColor(bgCo);
                    rb.setTextColor(txtCo);
                }
            }
        );
    }

    void updateColors(Color bgCo, Color txtCo)
    {
        for_each(
            this->rbtns.begin(),
            this->rbtns.end(),
            [bgCo, txtCo](RPressButton& rb) {
                rb.setBackgroundColor(bgCo);
                rb.setTextColor(txtCo);
            }
        );
    }

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
    Counter<100> beginXActions;
};

typedef unordered_map<string, DockActionSupport> DockActions;
typedef unordered_map<string, RPressButton> DockMenu;
const Color MENU_BLACK { 20,20,20, 200 };
struct Dock
{
    DockMenu menu;
    DockActions actions;
    string currentMenu = "";
    Counter<17> beginYMenu;
    bool isVisible = true;

    void addSectionMenu(string menuKey, string menuName)
    {
        if (!this->menu.contains(menuKey))
        {
            RPressButton rbtn { (float) (GetRenderWidth()-(MeasureText(menuName.c_str(), 17)+10)), (float) beginYMenu(), (float) (MeasureText(menuName.c_str(), 17)+5), 17.0, BLACK,
                [this, menuKey](RPressButton& _) {
                    this->hideAllActions(); // Hide all actions menus
                    this->selectSectionMenu(menuKey);

                    for (auto& acts : this->actions) // show logical group that respect current key
                    {
                        if (acts.second.originMenu == menuKey)
                        {
                            acts.second.rbtns.showAll();
                            break;
                        }
                    }
                },
            menuName, 17, WHITE };

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
        float sizeOfText = (float) (MeasureText(actName.c_str(), 17)+15);

        if (this->actions.contains(menuKey))
        {
            RPressButton rbtn { (float) this->actions[menuKey].beginXActions(), (float) (GetRenderHeight()-100), 100, 70.0, MENU_BLACK,
                [actKey, cb](RPressButton& rpb)
                {
                    if (rpb.getPayload() == actKey)
                        cb();
                }
            , actName, 15, WHITE };

            rbtn.setVisibility(false);
            rbtn.setPayload(actKey);

            this->actions[menuKey].rbtns.push(rbtn);
        } else
        {
            RPressButton rbtn { 0.0, (float) (GetRenderHeight()-100), 100, 70.0, MENU_BLACK,
                [actKey, cb](RPressButton& rpb)
                {
                    if (rpb.getPayload() == actKey)
                        cb();
                }
            , actName, 15, WHITE };

            rbtn.setVisibility(false);
            rbtn.setPayload(actKey);

            this->actions.insert_or_assign(
                menuKey,
                DockActionSupport {
                    menuKey,
                    RPressButtonLogicalGroup {
                        vector<RPressButton>{ {rbtn} }
                    },
                    Counter<100>{ (100) } });
        }
    }

    void hideAllActions() {
        for (auto& acts : this->actions)
            acts.second.rbtns.hideAll();
    }

    void draw()
    {
        for (auto& sectionMenu : this->menu)
        {
            if(this->currentMenu == sectionMenu.first)
            {
                sectionMenu.second.setBackgroundColor(YELLOW);
                sectionMenu.second.setTextColor(BLACK);
            } else
            {
                sectionMenu.second.setTextColor(WHITE);
                sectionMenu.second.setBackgroundColor(BLACK);
            }
            sectionMenu.second.draw();
        }

        for (auto& acts : this->actions)
            acts.second.rbtns.drawAll();
    }

    void pressDetection()
    {
        for (auto& sectionMenu : this->menu)
            sectionMenu.second.executeIfPressed();

        for (auto& acts : this->actions)
        {
            if (this->currentMenu == acts.first)
            {
                string pressedOne = acts.second.rbtns.pressDetection();

                if(pressedOne != "unknown")
                    acts.second.rbtns.updateColorsByPayload(YELLOW, MENU_BLACK, pressedOne);
                else
                    acts.second.rbtns.updateColors(MENU_BLACK, WHITE);
            }
        }
    }
};

enum DockTools { Normal, SandPillar, SandBucket }; // define existing tools
enum DockRayMode { OnePerOne, Compulsive };        // define mouse pression mode detecton (Compulsive=IsMouseButtonDown, OnePerOne=IsMouseButtonPressed)
struct DockRay
{
    Vec3I targetedVoxel{0,0,0};             // voxel targeted by Dock ray
    BlockType dockSelectedBlock = SAND;     // the selected block in ui
    DockRayMode state = Compulsive;
    DockTools tool = Normal;

    template<int X, int Y, int Z>
    void rayTest(Int3DSpace<X,Y,Z>& sworld, Camera& camera, Dock& dockMenu) // we take the closest block to camera
    {
        Ray ray = { 0 };
        RayCollision collision = { 0 };

        if (
            (state == Compulsive ? IsMouseButtonDown : IsMouseButtonPressed)(MOUSE_BUTTON_LEFT)
        )
        {
            ray = GetScreenToWorldRay(GetMousePosition(), camera);
            collision = {};

            for (int j = 0; j < Y; j++)
            {
                for (int i = 0; i < X; i++)
                {
                    for (int k = 0; k < Z; k++)
                    {
                        if (sworld.blockConst(i, j, k) == AIR)
                            continue;

                        Vector3 pos = Vector3{
                            (float)i, (float)j, (float)k
                        } + sworld.getDrawShift();

                        BoundingBox box{
                            pos,
                            Vector3{pos.x + 1, pos.y + 1, pos.z + 1}
                        };

                        RayCollision hit = GetRayCollisionBox(ray, box);

                        if (hit.hit &&
                            (!collision.hit || (hit.distance < collision.distance)))
                        {
                            collision = hit;
                            this->targetedVoxel.x = i;
                            this->targetedVoxel.y = j;
                            this->targetedVoxel.z = k;
                        }
                    }
                }
            }

            if (collision.hit)
            {
                switch(tool)
                {
                    case Normal:
                    {
                        if ((this->targetedVoxel.y + 1) >= Y)
                            return;

                        sworld.block(this->targetedVoxel + Vec3I { 0, 1, 0}) = this->dockSelectedBlock;
                        break;
                    }
                    case SandPillar:
                    {
                        sworld.block(this->targetedVoxel + Vec3I { 0, (Y-1)-this->targetedVoxel.y, 0}) = SAND;
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }
};

Dock buildMenu(DockRay& dockray)
{
    Dock dock;

    dock.addSectionMenu("blocks", "Blocks selection");
    dock.addSectionMenu("tools", "Tools");
    dock.addSectionMenu("settings", "Interaction Settings");

    dock.addAction("blocks", "blocks::sand", "Sand (1)", [&dockray]() { dockray.dockSelectedBlock = SAND; });
    dock.addAction("blocks", "blocks::air", "Air (2)", [&dockray]() { dockray.dockSelectedBlock = AIR; });
    dock.addAction("blocks", "blocks::dirt", "Dirt (3)", [&dockray]() { dockray.dockSelectedBlock = DIRT; });

    dock.addAction("tools", "tools::normal", "Place Block\nSelected", [&dockray]() { dockray.tool = Normal; });
    dock.addAction("tools", "tools::sandbucket", "Sand\nBucket", [&dockray]() {  dockray.tool = SandBucket; });
    dock.addAction("tools", "tools::sandpillar", "Sand\nPillar", [&dockray]() { dockray.tool = SandPillar; });

    dock.addAction("settings", "settings::interaction::compulsive", "Selection\nCompulsive", [&dockray]() { dockray.state = Compulsive; });
    dock.addAction("settings", "settings::interaction::oneperone", "Selection\nUnique", [&dockray]() { dockray.state = OnePerOne; });

    return dock;
}