#pragma once

#include "blocks.hpp"
#include "i3dvector.hpp"

struct LogicalVoxel {
public:
    Vec3I localPos;
    BlockType type;

    LogicalVoxel(Vec3I lp, BlockType bt) :  localPos { lp }, type { bt } {}
};

struct VoxelLogicalGroup
{
public:
    inline static int count = 0;
    int id;
    Vec3I origin;
    vector<LogicalVoxel> mask; // the schematic around origin vec {{(0,1,1),WOOD}, {(-1,-1,0),STONE}, ...}

    VoxelLogicalGroup(Vec3I o, vector<LogicalVoxel> schematic) : 
        id { VoxelLogicalGroup::count++ }, origin { o }, mask { schematic }  {}
    VoxelLogicalGroup() : VoxelLogicalGroup { Vec3I { 0,0,0 }, vector<LogicalVoxel>{} } {}

    Vec3I getDimensions() const
    {
        if (this->mask.empty())
            return {0, 0, 0};

        Vec3I min = this->mask[0].localPos;
        Vec3I max = this->mask[0].localPos;

        for (const LogicalVoxel& lv : this->mask)
        {
            const Vec3I& p = lv.localPos;

            min.x = std::min(min.x, p.x);
            min.y = std::min(min.y, p.y);
            min.z = std::min(min.z, p.z);

            max.x = std::max(max.x, p.x);
            max.y = std::max(max.y, p.y);
            max.z = std::max(max.z, p.z);
        }

        return {
            max.x - min.x + 1,
            max.y - min.y + 1,
            max.z - min.z + 1
        };
    }
};

// parsing strings to build furnitures 
// strings of config is y=n+origin_y wich n is the local pos around origin
VoxelLogicalGroup createFromMask(Vec3I at, vector<string> config)
{
    vector<LogicalVoxel> schematic;

    int curX = 0;
    int curY = 0;
    int curZ = 0;

    for (const string & atdim : config)
    {
        curX = 0;
        curZ = 0;

        for (char c : atdim)
        {
            if (c == ':')
            {
                curZ++;
                curX = 0;
                continue;
            }

            Vec3I pos{curX, curY, curZ};

            switch (c)
            {
                case 'S':
                    schematic.push_back(
                        LogicalVoxel{
                            pos,
                            SAND
                        }
                    );
                    break;
                case 'W':
                    schematic.push_back(
                        LogicalVoxel{
                            pos,
                            WATER
                        }
                    );
                    break;
                case 'w':
                    schematic.push_back(
                        LogicalVoxel{
                            pos,
                            WOOD
                        }
                    );
                    break;
                case 'd':
                    schematic.push_back(
                        LogicalVoxel{
                            pos,
                            DIRT
                        }
                    );
                    break;
                case 's':
                    schematic.push_back(
                        LogicalVoxel{
                            pos,
                            STONE
                        }
                    );
                    break;
            }

            curX++;
        }

        curY++;
    }

    return VoxelLogicalGroup{at, schematic};
}

VoxelLogicalGroup createChair(Vec3I at) 
{
    return createFromMask(at, vector<string> { { 
            "s00s:0000:s00s", // foot
            "s00s:0000:s00s", // foot
            "s00s:0000:s00s", // foot
            "ssss:wwww:wwww", // seat
            "s00s:0000:0000", // back
            "ssss:0000:0000", // back
            "s00s:0000:0000", // back
            "ssss:0000:0000", // back
    } });
}


VoxelLogicalGroup createTable(Vec3I at)
{
    return createFromMask(at, vector<string> { { 
            "s000s:00000:00000:s000s", // foot
            "s000s:00000:00000:s000s", // foot
            "s000s:00000:00000:s000s", // foot
            "s000s:00000:00000:s000s", // foot
            "wwwww:wwwww:wwwww:wwwww", // plate
    } });
}

VoxelLogicalGroup createGiantFoot(Vec3I at) 
{
    return createFromMask(at, vector<string>{
        "00000ssssss00000:" // c'est assez voyant...
        "0000ssssssss0000:"
        "0000ssssssss0000:"
        "0000ssssssss0000:"
        "0000ssssssss0000:"
        "00000sssssss0000:"
        "00000sssssss0000:"
        "00000ssssssss000:"
        "0000sssssssss000:"
        "0000ssssssssss00:"
        "000sssssssssss00:"
        "00sssssssssssss0:"
        "00sssssssssssss0:"
        "0ssssssssssssss0:"
        "0ssssssssssssss0:"
        "0ssssssssssssss0:"
        "0sss0ss0ss0ss0s0:"
        "0sss0ss0ss0ss000:"
        "0sss0ss0ss000000:"
        "0sss000000000000",

        "00000ssssss00000:"
        "0000ssssssss0000:"
        "0000ssssssss0000:"
        "0000ssssssss0000:"
        "0000ssssssss0000:"
        "00000sssssss0000:"
        "00000sssssss0000:"
        "00000ssssssss000:"
        "0000sssssssss000:"
        "00000ssssssss000:"
        "0000sssssssss000:"
        "000sssssssssss00:"
        "000sssssssssss00:"
        "00ssssssssssss00:"
        "00ssssssssssss00:"
        "00ssssssssssss00:"
        "0sss0ss0ss0ss0s0:"
        "0sss0ss0ss0ss000:"
        "0sss0ss0ss000000:"
        "0sss000000000000",

        "000000ssss000000:"
        "00000ssssss00000:"
        "00000ssssss00000:"
        "00000ssssss00000:"
        "00000ssssss00000:"
        "000000sssss00000:"
        "000000sssss00000:"
        "000000ssssss0000:"
        "00000sssssss0000:"
        "00000ssssssss000:"
        "0000sssssssss000:"
        "000sssssssssss00:"
        "000sssssssssss00:"
        "00ssssssssssss00:"
        "00ssssssssssss00:"
        "00ssssssssssss00:"
        "0sss0ss0ss0ss000:"
        "0sss0ss0ss000000:"
        "0sss000000000000:"
        "0000000000000000",

        "000000ssss000000:"
        "00000ssssss00000:"
        "00000ssssss00000:"
        "00000ssssss00000:"
        "00000ssssss00000:"
        "000000sssss00000:"
        "000000sssss00000:"
        "000000ssssss0000:"
        "00000sssssss0000:"
        "000000ssssss0000:"
        "00000sssssss0000:"
        "0000sssssssss000:"
        "0000sssssssss000:"
        "000ssssssssss000:"
        "000ssssssssss000:"
        "000ssssssssss000",

        "0000000000000000:"
        "0000000000000000:"
        "0000000000000000:"
        "000000ssss000000:"
        "000000ssss000000:"
        "0000000sss000000:"
        "0000000sss000000:"
        "0000000ssss00000:"
        "000000sssss00000:"
        "000000ssssss0000:"
        "00000sssssss0000:"
        "0000sssssssss000:"
        "0000sssssssss000",

        "0000000000000000:"
        "0000000000000000:"
        "0000000000000000:"
        "0000000000000000:"
        "0000000000000000:"
        "0000000000000000:"
        "0000000sss000000:"
        "0000000ssss00000:"
        "000000sssss00000:"
        "0000000ssss00000:"
        "000000sssss00000"
    });
}

VoxelLogicalGroup createBucket(Vec3I at)
{
    return createFromMask(at, vector<string>{
        "ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss",
        "ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss:ssssssssssssssssssss",

        "ssssssssssssssssssss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssssssssssssssssssss",
        "ssssssssssssssssssss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssssssssssssssssssss",
        "ssssssssssssssssssss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssssssssssssssssssss",
        "ssssssssssssssssssss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssssssssssssssssssss",
        "ssssssssssssssssssss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssssssssssssssssssss",
        "ssssssssssssssssssss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssssssssssssssssssss",
        "ssssssssssssssssssss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssssssssssssssssssss",
        "ssssssssssssssssssss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssssssssssssssssssss",
        "ssssssssssssssssssss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssssssssssssssssssss",
        "ssssssssssssssssssss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssssssssssssssssssss",
        "ssssssssssssssssssss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWW:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssssssssssssssssssss",
        "ssssssssssssssssssss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssssssssssssssssssss",
        "ssssssssssssssssssss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssWWWWWWWWWWWWWWWWss:ssssssssssssssssssss",

        "ssssssssssssssssssss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ssssssssssssssssssss",
        "ssssssssssssssssssss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ssssssssssssssssssss",

        "ssss0sssssssssssssss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ssssssssssssssssssss",

        "ssssssssssssssssssss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ssssssssssssssssssss",
        "ssssssssssssssssssss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ss0000000000000000ss:ssssssssssssssssssss"
    });
}