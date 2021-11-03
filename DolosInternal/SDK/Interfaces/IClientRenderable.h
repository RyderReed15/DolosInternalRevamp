#pragma once

#ifndef CLIENT_RENDERABLE_H
#define CLIENT_RENDERABLE_H

#include "../Math/Vector.h"

struct model_t;
class IClientUnknown;
class IHandleEntity;

class IClientRenderable
{
public:
    virtual IClientUnknown*     GetIClientUnknown(void) = 0;
    virtual Vector const&       GetRenderOrigin(void) = 0;
    virtual Vector const&       GetRenderAngles(void) = 0;
    virtual bool                ShouldDraw(void) = 0;
    virtual int                 GetRenderFlags(void) = 0;

    virtual bool                Unknown000(void) = 0;
    virtual void                GetShadowHandle(void) const = 0;
    virtual IHandleEntity       RenderHandle(void) = 0;
    virtual model_t*            GetModel(void) const = 0;
    virtual int                 DrawModel(int flags) = 0;
    virtual int                 GetBody(void) = 0;
    virtual void                ComputeFxBlend(void) = 0;
};

#endif