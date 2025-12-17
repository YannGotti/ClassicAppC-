#pragma once
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>

using namespace Gdiplus;

class IRenderable
{
public:
    virtual ~IRenderable() = default;
    virtual void Draw(Gdiplus::Graphics& g) { /**/ };
    virtual void DrawScreen(Gdiplus::Graphics& g, const RECT& clientRect) { /**/ };
    
    virtual void Update(float deltaTime) = 0;

    virtual bool IsDynamic() const { return false; }
};
