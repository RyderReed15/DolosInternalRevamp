#pragma once

#ifndef BASE_CLIENT_H
#define BASE_CLIENT_H


#include "../Math/Vector.h"
#include "../Math/QAngle.h"

class CreateInterfaceFn;
class IClientNetworkable;
typedef IClientNetworkable* (*CreateClientClassFn)(int entnum, int serialNum);
typedef IClientNetworkable* (*CreateEventFn)();
class ClassId;

class RecvTable;
class RecvProp;

#define CKNIFE 107

class IClientClass
{
public:
    CreateClientClassFn         m_pCreateFn;
    CreateEventFn               m_pCreateEventFn;
    char*                       m_pNetworkName;
    RecvTable*                  m_pRecvTable;
    IClientClass*               m_pNext;
    int                         m_ClassID;
};
class RecvProp {
public:
    char*       m_pVarName;
    int         m_RecvType;
    int         m_Flags;
    int         m_StringBufferSize;
    int         m_bInsideArray;
    const int   m_pExtraData;
    RecvProp*   m_pArrayProp;
    int         m_ArrayLengthProxy;
    int         m_ProxyFn;
    int         m_DataTableProxyFn;
    RecvTable*  m_pDataTable;
    int         m_Offset;
    int         m_ElementStride;
    int         m_nElements;
    const int   m_pParentArrayPropName;
};

class RecvTable {
public:
    RecvProp*   m_pProps;
    int         m_nProps;
    int         m_pDecoder;
    char*       m_pNetTableName;
    bool        m_bInitialized;
    bool        m_bInMainList;
};



class CGlobalVarsBase;



#define IN_ATTACK       (1 << 0)
#define IN_JUMP         (1 << 1)
#define IN_DUCK         (1 << 2)
#define IN_FORWARD      (1 << 3)
#define IN_BACK         (1 << 4)
#define IN_USE          (1 << 5)
#define IN_CANCEL       (1 << 6)
#define IN_LEFT         (1 << 7)
#define IN_RIGHT        (1 << 8)
#define IN_MOVELEFT     (1 << 9)
#define IN_MOVERIGHT    (1 << 10)
#define IN_ATTACK2      (1 << 11)
#define IN_RUN          (1 << 12)
#define IN_RELOAD       (1 << 13)
#define IN_ALT1         (1 << 14)
#define IN_ALT2         (1 << 15)
#define IN_SCORE        (1 << 16)   // Used by client.dll for when scoreboard is held down
#define IN_SPEED        (1 << 17) // Player is holding the speed key
#define IN_WALK         (1 << 18) // Player holding walk key
#define IN_ZOOM         (1 << 19) // Zoom key for HUD zoom
#define IN_WEAPON1      (1 << 20) // weapon defines these bits
#define IN_WEAPON2      (1 << 21) // weapon defines these bits
#define IN_BULLRUSH     (1 << 22)
#define IN_GRENADE1     (1 << 23) // grenade 1
#define IN_GRENADE2     (1 << 24) // grenade 2
#define IN_LOOKSPIN     (1 << 25)

class CUserCmd
{
public:

    virtual ~CUserCmd() {};



    int     iCommandNumber;     // 0x04 For matching server and client commands for debugging
    int     iTickCount;         // 0x08 the tick the client created this command
    QAngle  qViewAngles;         // 0x0C Player instantaneous view angles.
    Vector  vAimDirection;       // 0x18
    float   flForwardMove;        // 0x24
    float   flSideMove;           // 0x28
    float   flUpMove;             // 0x2C
    int     iButtons;            // 0x30 Attack button states
    char    bImpulse;            // 0x34
    int     iWeaponSelect;       // 0x38 Current weapon id
    int     iWeaponSubType;      // 0x3C
    int     iRandomSeed;        // 0x40 For shared random functions
    short   sMouseDX;            // 0x44 mouse accum in x from create move
    short   sMouseDY;            // 0x46 mouse accum in y from create move
    bool    bHasBeenPredicted;   // 0x48 Client only, tracks whether we've predicted this command at least once
    char    pad_0x4C[0x18];     // 0x4C Current sizeof( usercmd ) =  100  = 0x64
};

enum class ClientFrameStage_t
{
    FRAME_UNDEFINED = -1,
    FRAME_START,
    FRAME_NET_UPDATE_START,
    FRAME_NET_UPDATE_POSTDATAUPDATE_START,
    FRAME_NET_UPDATE_POSTDATAUPDATE_END,
    FRAME_NET_UPDATE_END,
    FRAME_RENDER_START,
    FRAME_RENDER_END
};

// Used by RenderView
enum RenderViewInfo_t
{
    RENDERVIEW_UNSPECIFIED = 0,
    RENDERVIEW_DRAWVIEWMODEL = (1 << 0),
    RENDERVIEW_DRAWHUD = (1 << 1),
    RENDERVIEW_SUPPRESSMONITORRENDERING = (1 << 2),
};

class IBaseClientDLL
{
public:
    virtual int              Connect(CreateInterfaceFn appSystemFactory, CGlobalVarsBase* pGlobals) = 0;
    virtual int              Disconnect(void) = 0;
    virtual int              Init(CreateInterfaceFn appSystemFactory, CGlobalVarsBase* pGlobals) = 0;
    virtual void             PostInit() = 0;
    virtual void             Shutdown(void) = 0;
    virtual void             LevelInitPreEntity(char const* pMapName) = 0;
    virtual void             LevelInitPostEntity() = 0;
    virtual void             LevelShutdown(void) = 0;
    virtual IClientClass*    GetAllClasses(void) = 0;


};




#endif // !BASE_CLIENT_H
