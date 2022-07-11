#pragma once

#ifndef RENDER_H
#define RENDER_H


#include <d3d9.h>
#include <d3dx9core.h>
#include <d3dx9tex.h>
#include "../resource.h"

#define D3DFVF_CUSTOMVERTEX             (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
#define D3DFVF_CUSTOMVERTEX_TEXTURE     (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
#define PI                              3.14159265358979323846f
#define RAD_TO_DEG                      57.2957795130823208767f // 180 / PI
#define DEG_TO_RAD                      0.01745329251994329576f // PI / 180

#define ROUND_CORNER_LOD                3

#define EMPTY       0x00000000

#define BLACK       0xff000000
#define DARKGRAY    0xff111111
#define LIGHTGRAY   0xff222222
#define OFFWHITE    0xffeeeeee
#define WHITE       0xffffffff
#define GRAY        0xffaaaaaa

#define RED         0xffff0000
#define PINK        0xffcf5880
#define MAGENETA    0xffff00ff
#define BLUE        0xff0000ff
#define LIGHTBLUE   0xff40b0ff
#define CYAN        0xff00ffff
#define GREEN       0xff00ff00
#define YELLOW      0xffffff00

#define TRANSPARENT_GRAY    0x33333333



enum class BUFFER_TYPE {
    BUFFER_ALL,
    BUFFER_LINE,
    BUFFER_TRI,
    BUFFER_TEXTURE,
    BUFFER_TEXT
};



struct CustomVertex
{
    float       x;
    float       y;
    float       z;
    float       rhw;
    D3DCOLOR    color;
};
struct CustomTextureVertex
{
    float       x;
    float       y;
    float       z;
    float       rhw;
    D3DCOLOR    color;
    float       u, v;
};


class Render {
public:
                        Render                  (void);
                        Render                  (IDirect3DDevice9* pDevice, HMODULE hMod);
                        ~Render                 (void);
    void                Initialize              (IDirect3DDevice9* pDevice, HMODULE hMod);

    void                Release                 (void);
    void                Begin                   (BUFFER_TYPE tBufferType = BUFFER_TYPE::BUFFER_ALL);
    void                End                     (BUFFER_TYPE tBufferType = BUFFER_TYPE::BUFFER_ALL);

    IDirect3DDevice9*   GetDevice               (void);
    bool                IsInitialized           (void);

    void                ManageBatch             (BUFFER_TYPE tBufferType, unsigned int iNeededVerts, unsigned int iNeededIndices);
    unsigned int        AddVertex               (BUFFER_TYPE tBufferType, D3DXVECTOR2 vLocation, D3DCOLOR cColor, D3DXVECTOR2 vTexCoords = {});
    void                AddIndex                (BUFFER_TYPE tBufferType, unsigned int iVertexPos);
    void                AddTriangle             (unsigned int iVertexPosOne, unsigned int iVertexPosTwo, unsigned int iVertexPosThree);
    void                AddTexTriangle          (unsigned int iVertexPosOne, unsigned int iVertexPosTwo, unsigned int iVertexPosThree);

    void                SetTexture              (IDirect3DTexture9* pTexture);
    IDirect3DTexture9*  LoadTexture             (const char* szPath);

   

    HRESULT             DrawSprite              (D3DXVECTOR4 vRect, D3DXVECTOR2 vLocation, D3DCOLOR cColor, float flScale = 1.f, float flRotation = 0.f);
    HRESULT             DrawLine                (D3DXVECTOR2 vLocationOne, D3DXVECTOR2 vLocationTwo, D3DCOLOR cColor, D3DCOLOR cColor2 = 0);
    HRESULT             DrawString              (D3DXVECTOR2 vLocation, D3DCOLOR cColor, ID3DXFont* pFont, const char* szString, ...);
    HRESULT             DrawString              (D3DXVECTOR2 vLocation, D3DCOLOR cColor, ID3DXFont* pFont, const WCHAR* szString, ...);
    D3DXVECTOR2         GetStringSize           (ID3DXFont* font, const WCHAR* string, ...);
    D3DXVECTOR2         GetStringSize           (ID3DXFont* font, const char* string, ...);
    HRESULT             DrawRectangle           (D3DXVECTOR4 vBounds, D3DCOLOR cColor, D3DCOLOR cColor2 = 0, bool bVertical = true);
    HRESULT             DrawTextureRectangle    (D3DXVECTOR4 vBounds, D3DXVECTOR4 vTexCoords);
    HRESULT             DrawOutlinedRect        (D3DXVECTOR4 vBounds, unsigned int iThickness, D3DCOLOR cOutline, D3DCOLOR cColor, D3DCOLOR cColor2 = 0, bool bVertical = true);
    HRESULT             DrawCircle              (D3DXVECTOR2 vLocation, float flRadius, unsigned int iSides, D3DCOLOR cColor, float flFraction = 1, float flRotation = 0);
    HRESULT             DrawTextureCircle       (D3DXVECTOR2 vLocation, float flRadius, unsigned int iSides, D3DXVECTOR4 vTexCoords, float flFraction = 1, float flRotation = 0);
    HRESULT             DrawCircleOutline       (D3DXVECTOR2 vLocation, float flRadius, unsigned int iSides, D3DCOLOR cColor, float flFraction = 1, float flRotation = 0);
    HRESULT             DrawRoundedRectangle    (D3DXVECTOR4 vBounds, float flCornerSize, D3DCOLOR cColor, D3DCOLOR cColor2 = 0, bool bVertical = true);
private:
    HRESULT             DrawFadingCircle        (D3DXVECTOR2 vLocation, float flRadius, unsigned int iSides, D3DCOLOR cColor, D3DCOLOR cColor2, bool bVertical = true, float flFraction = 1, float flRotation = 0);

    CustomVertex*           m_pLineVertex, * m_pTriVertex;
    CustomTextureVertex*    m_pTextureVertex;
    unsigned int*           m_pLineIndex, * m_pTriIndex, * m_pTextureIndex;
    unsigned int            m_iMaxVertices;
    unsigned int            m_iLineCount, m_iTriangleCount, m_iTextureCount;
    unsigned int            m_iLineIndices, m_iTriangleIndices, m_iTextureIndices;
    unsigned int            m_iLineVertices, m_iTriangleVertices, m_iTextureVertices;
    IDirect3DDevice9*       m_pDevice;
    IDirect3DVertexBuffer9* m_pLineVertexBuffer, * m_pTriVertexBuffer, * m_pTextureVertexBuffer;
    IDirect3DIndexBuffer9*  m_pLineIndexBuffer, * m_pTriIndexBuffer, * m_pTextureIndexBuffer;
    ID3DXSprite*            m_pSprite;
    IDirect3DTexture9*      m_pTextureAtlas;
};

D3DCOLOR            LerpColor(D3DCOLOR cColorOne, D3DCOLOR cColorTwo, float flPercent);
D3DCOLOR            LerpAlpha(D3DCOLOR cColor, float flPercent, bool bToZero = false);


#endif // !RENDER_H
