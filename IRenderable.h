#pragma once
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>

using namespace Gdiplus;

class IRenderable
{
public:
    virtual ~IRenderable() = default;
    virtual void Draw(Gdiplus::Graphics& g) = 0;

    virtual bool IsDynamic() const { return false; }
};
