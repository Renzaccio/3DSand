#pragma once

#include <algorithm>
#include <functional>
#include "counter.hpp"
#include "i3dspace.hpp"
#include "i3dvector.hpp"

using namespace std;

struct RPressButton : Rectangle
{
private:

    bool             isVisible = true;
    Color            color{ 255,255,255,255 };
    string           text = "";
    int              textSize;
    Color            textColor{ 0,0,0,255 };
    function<void(RPressButton &)> callback; // a acces a l'object
    string           payload = "unknown";

public:
    RPressButton(float x, float y, float width, float height, Color col, function<void(RPressButton &)> cb, string txt, int txtSize, Color txtCol) :
        Rectangle{ x,y,width,height }, color{ col }, callback{ cb }, text(txt), textSize{ txtSize }, textColor{ txtCol } {}

    void draw()               const { this->draw(this->x, this->y); }
    void draw(int yY)         const { this->draw(this->x, yY); }
    void draw(int xX, int yY) const
    {
        const Rectangle bounds{ (float)xX, (float)yY, this->width, this->height };
        const bool hovered = this->isVisible && CheckCollisionPointRec(GetMousePosition(), bounds);
        const Color backgroundColor = hovered ? YELLOW : this->color;
        const Color foregroundColor = hovered ? BLACK : this->textColor;

        DrawRectangle(xX, yY, this->width+2, this->height+2, BLACK);
        DrawRectangle(xX, yY, this->width, this->height, backgroundColor);

        int txtRealWidth = MeasureText(this->text.c_str(), this->textSize);
        int txtHeight = this->textSize;

        int posX = xX + ((this->width - txtRealWidth)/2);
        int posY = yY + ((this->height - txtHeight)/2);

        DrawText(this->text.c_str(), posX, posY, this->textSize, foregroundColor);
    }

    string getPayload() const  { return this->payload; }
    void setPayload(string pa) { this->payload = pa; }

    void setBackgroundColor(Color co) { this->color = co; }
    void setTextColor(Color co)       { this->textColor = co; }

    bool buttonIsVisible() const { return this->isVisible; }
    void setVisibility(bool vb)  { this->isVisible = vb; }

    bool executeIfPressed()
    {
        if (CheckCollisionPointRec(GetMousePosition(), *this))
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                this->callback(*this);
                return true;
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
    RPressButtonLogicalGroup(vector<RPressButton> rbs) : rbtns{ std::move(rbs) } {}

    void push(RPressButton btn)
    {
        this->rbtns.push_back(btn);
    }

    void drawAll()
    {
        for (RPressButton & rb : rbtns)
            if (rb.buttonIsVisible())
                rb.draw();
    }

    string pressDetection()
    {
        string lastexec = "unknown";

        for (RPressButton & rb : rbtns)
            if (rb.buttonIsVisible())
                if (rb.executeIfPressed())
                {
                    lastexec = rb.getPayload();
                }

        return lastexec;
    }

    bool isExist(int idx) { return (idx >= 0) && (idx < this->rbtns.size()); }

    void updateColorsByPayload(Color bgCo, Color txtCo, string payload)
    {
        for_each(
            this->rbtns.begin(),
            this->rbtns.end(),
            [payload, bgCo, txtCo](RPressButton & rb) {
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
            [bgCo, txtCo](RPressButton & rb) {
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
            [](RPressButton & rb) {
                rb.setVisibility(false);
            }
        );
    }

    void showAll()
    {
        for_each(
            this->rbtns.begin(),
            this->rbtns.end(),
            [](RPressButton & rb) {
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
                [this, menuKey](RPressButton & _) {
                    this->hideAllActions(); // Hide all actions menus
                    this->selectSectionMenu(menuKey);

                    for (auto & acts : this->actions) // show logical group that respect current key
                        if (acts.second.originMenu == menuKey)
                        {
                            acts.second.rbtns.showAll();
                            break;
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
                [actKey, cb](RPressButton & rpb)
                {
                    if (rpb.getPayload() == actKey)
                        cb();
                },
            actName, 15, WHITE };

            rbtn.setVisibility(false);
            rbtn.setPayload(actKey);

            this->actions[menuKey].rbtns.push(rbtn);
        }
        else
        {
            RPressButton rbtn { 0.0, (float) (GetRenderHeight()-100), 100, 70.0, MENU_BLACK,
                [actKey, cb](RPressButton & rpb)
                {
                    if (rpb.getPayload() == actKey)
                        cb();
                },
            actName, 15, WHITE };

            rbtn.setVisibility(false);
            rbtn.setPayload(actKey);

            this->actions.insert_or_assign(
                menuKey,
                DockActionSupport {
                    menuKey,
                    RPressButtonLogicalGroup {
                        vector<RPressButton>{ { rbtn } }
                    },
                    Counter<100>{ (100) }
                }
            );
        }
    }

    void hideAllActions() {
        for (auto & acts : this->actions)
            acts.second.rbtns.hideAll();
    }

    void draw()
    {
        for (auto & sectionMenu : this->menu)
        {
            if (this->currentMenu == sectionMenu.first)
            {
                sectionMenu.second.setBackgroundColor(YELLOW);
                sectionMenu.second.setTextColor(BLACK);
            }
            else
            {
                sectionMenu.second.setTextColor(WHITE);
                sectionMenu.second.setBackgroundColor(BLACK);
            }
            sectionMenu.second.draw();
        }

        for (auto & acts : this->actions)
            acts.second.rbtns.drawAll();
    }

    void pressDetection()
    {
        for (auto & sectionMenu : this->menu)
            sectionMenu.second.executeIfPressed();

        for (auto & acts : this->actions)
            if (this->currentMenu == acts.first)
            {
                string pressedOne = acts.second.rbtns.pressDetection();

                if (pressedOne != "unknown")
                    acts.second.rbtns.updateColorsByPayload(YELLOW, MENU_BLACK, pressedOne);
                else
                    acts.second.rbtns.updateColors(MENU_BLACK, WHITE);
            }
    }
};

enum DockTools { NORMAL, RAIN }; // define existing tools
enum DockRayMode { CLICK, HOLD };        // define mouse pression mode detecton (Compulsive=IsMouseButtonDown, OnePerOne=IsMouseButtonPressed)
struct DockRay
{
    Vec3I targetedPos{ 0,0,0 };             // voxel targeted by Dock ray
    BlockType dockSelectedBlock = SAND;     // the selected block in ui
    DockRayMode state = HOLD;
    DockTools tool = NORMAL;

    template<int WLEN, int HLEN, int DLEN>
    void rayTest(Int3DSpace<WLEN, HLEN, DLEN> & sworld, Camera & camera, Dock & dockMenu) // we take the closest block to camera
    {
        Ray ray = { 0 };
        RayCollision collision = { 0 };

        ray = GetScreenToWorldRay(GetMousePosition(), camera);
        collision = {};

        sworld.voxel(this->targetedPos).isTargeted = false;

        for (int x = 0; x < WLEN; x++)
            for (int y = 0; y < HLEN; y++)
                for (int z = 0; z < DLEN; z++)
                {
                    if (sworld.blockConst(x, y, z) == AIR)
                        continue;

                    Vector3 pos = Vector3{
                        (float)x, (float)y, (float)z
                    } + sworld.getDrawShift();

                    BoundingBox box{
                        pos,
                        pos + Vector3{ 1, 1, 1 }
                    };

                    RayCollision hit = GetRayCollisionBox(ray, box);

                    if (hit.hit &&
                        (!collision.hit || (hit.distance < collision.distance)))
                    {
                        collision = hit;
                        this->targetedPos.x = x;
                        this->targetedPos.y = y;
                        this->targetedPos.z = z;
                    }
                }

        if (collision.hit)
        {
            Voxel& vx = sworld.voxel(this->targetedPos);
            vx.isTargeted = true;

            if ((state == HOLD ? IsMouseButtonDown :  IsMouseButtonPressed)(MOUSE_BUTTON_LEFT))
            {
                Vec3I posToEdit = this->targetedPos;
                switch(this->tool)
                {
                    case NORMAL:
                    {
                        Vector3 norm = collision.normal;
                        int newX = posToEdit.x + (int) roundf(norm.x);
                        int newY = posToEdit.y + (int) roundf(norm.y);
                        int newZ = posToEdit.z + (int) roundf(norm.z);
                        Vec3I blockNearSelectedBlock {newX, newY, newZ};
                        
                        if (!sworld.isInBounds(blockNearSelectedBlock))
                            break;

                        sworld.block(blockNearSelectedBlock) = this->dockSelectedBlock;
                        sworld.activateAllNeighboursSurfaceVisibility(blockNearSelectedBlock);
                        sworld.activateNeighboursMobility(blockNearSelectedBlock);
                        break;
                    }
                    case RAIN:
                        posToEdit.y = HLEN - 1;
                        sworld.block(posToEdit) = this->dockSelectedBlock;
                        sworld.activateAllNeighboursSurfaceVisibility(posToEdit);
                        sworld.activateNeighboursMobility(posToEdit);
                        break;
                }
            }
        }
    }
};

Dock buildMenu(DockRay & dockray)
{
    Dock dock;

    dock.addSectionMenu("blocks", "Blocks selection");
    dock.addSectionMenu("tools", "Tools");
    dock.addSectionMenu("settings", "Interaction Settings");

    dock.addAction("blocks", "blocks::sand", "Sand (1)", [&dockray]() { dockray.dockSelectedBlock = SAND; });
    dock.addAction("blocks", "blocks::air", "Air (2)", [&dockray]() { dockray.dockSelectedBlock = AIR; });
    dock.addAction("blocks", "blocks::dirt", "Dirt (3)", [&dockray]() { dockray.dockSelectedBlock = DIRT; });

    dock.addAction("blocks", "blocks::water", "Water (4)", [&dockray]() { dockray.dockSelectedBlock = WATER; });
    dock.addAction("blocks", "blocks::wood", "Wood (5)", [&dockray]() { dockray.dockSelectedBlock = WOOD; });
    dock.addAction("blocks", "blocks::stone", "Stone (6)", [&dockray]() { dockray.dockSelectedBlock = STONE; });
    dock.addAction("blocks", "blocks::debug", "Debug (7)", [&dockray]() { dockray.dockSelectedBlock = DEBUG; });

    dock.addAction("tools", "tools::normal", "Place Block", [&dockray]() { dockray.tool = NORMAL; });
    dock.addAction("tools", "tools::rain", "Rain", [&dockray]() { dockray.tool = RAIN; });

    dock.addAction("settings", "settings::interaction::hold", "Selection\nHold", [&dockray]() { dockray.state = HOLD; });
    dock.addAction("settings", "settings::interaction::click", "Selection\nClick", [&dockray]() { dockray.state = CLICK; });

    return dock;
}