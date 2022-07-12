#pragma once

#ifndef ITEM_SCHEMA_H
#define ITEM_SCHEMA_H

#include "../../Utils/Macros.h"
#include "../Math/UtlMap.h"
#include "../Math/UtlString.h"

//https://github.com/perilouswithadollarsign/cstrike15_src/blob/f82112a2388b841d72cb62ca48ab1846dfcc11c8/game/shared/econ/econ_item_schema.h

class IItemSchema;

struct PaintKit;
struct StickerKit;

class IItemSystem {

    VFUNC(IItemSchema*, GetItemSchema, 0, (), (this));
};

class IItemSchema {
public:
    /*char _pad000[0x88];
    UtlMap<int, EconItemQualityDefinition> qualities;
    char _pad001[0x48];
    UtlMap<int, EconItemDefinition*> itemsSorted;
    char _pad002[0x60];
    UtlMap<int, const char*> revolvingLootLists;
    char _pad003[0x80];
    UtlMap<std::uint64_t, AlternateIconData> alternateIcons;
    char _pad004[0x48];
    UtlMap<int, PaintKit*> paintKits;
    UtlMap<int, StickerKit*> stickerKits;
    char _pad005[0x11C];
    UtlMap<int, EconMusicDefinition*> musicKits;*/
};


enum class EconRarity : std::uint8_t {
    Default = 0,
    Gray,
    LightBlue,
    Blue,
    Purple,
    Pink,
    Red,
    Gold
};

struct PaintKit {
    // Generic fields
    int iPaintKitID;
    UtlString szName;
    UtlString szDescription;
    UtlString szDescriptionTag; // Skin Name
    UtlString sameNameFamilyAggregate;

    // (Generally) Weapon paint kit fields
    // Note that some fields may affect item generation for other paint kit types;
    // in particular the wear settings.
    UtlString szPattern;
    UtlString szNormal;
    UtlString szLogoMat;
    bool bBaseDiffuseOverride;
    int iRarity;
    char _pad000[40]; // Colors and styles
    float flWearRemapMin;
    float flWearRemapMax;
};

struct StickerKit {
    int iStickerKitID;
    int iRarity;
    UtlString szName;
    UtlString szDesc;
    UtlString szItemName;
    UtlString szMaterialPath;
    UtlString szMaterialPathNoDrips;
    UtlString szInventoryImage;

    int iEventID;
    int iEventTeamID;
    int iEventPlayerID;
};


#endif // !ITEM_SCHEMA_H
