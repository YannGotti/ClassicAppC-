// CoordinateSystem.h
#pragma once
#include <windows.h>       
#include <objidl.h>        
#include <gdiplus.h>      
#pragma comment (lib,"Gdiplus.lib")

using namespace Gdiplus;

// Простая утилита для конвертации координат между миром и экраном.
// scale = zoom, offset = world coordinate of top-left corner of viewport.
struct CoordinateSystem
{
    CoordinateSystem(float scale = 1.0f, PointF offset = PointF(0, 0))
        : m_scale(scale), m_offset(offset) {
    }

    // World -> Screen (pixels)
    PointF WorldToScreen(const PointF& world) const {
        return PointF(
            (world.X - m_offset.X) * m_scale,
            (world.Y - m_offset.Y) * m_scale
        );
    }

    // Screen -> World
    PointF ScreenToWorld(const PointF& screen) const {
        return PointF(
            screen.X / m_scale + m_offset.X,
            screen.Y / m_scale + m_offset.Y
        );
    }

    void SetScale(float s) { m_scale = (s > 0.0f) ? s : 1.0f; }
    float GetScale() const { return m_scale; }

    void SetOffset(const PointF& o) { m_offset = o; }
    PointF GetOffset() const { return m_offset; }

private:
    float m_scale;
    PointF m_offset;
};