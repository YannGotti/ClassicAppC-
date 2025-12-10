#pragma once
#define NOMINMAX
#include <windows.h>       // Всегда первым!
#include <objidl.h>        // Для интерфейсов COM
#include <gdiplus.h>       // GDI+ заголовок
#pragma comment (lib,"Gdiplus.lib")
#include "CoordinateSystem.h"

class Entity; // forward

using namespace Gdiplus;

class Camera
{
public:
    Camera();

    // Follow an entity (camera centers on entity's center)
    void Follow(Entity* entity);

    // Update camera internal smoothing (call once per frame)
    void Update(float dt = 1.0f);

    // Set view size in pixels (client area)
    void SetViewSize(int width, int height);

    // Set smoothing 0..1 (1 = instant snap, 0 = no movement)
    void SetSmoothing(float smoothing);

    // Zoom setter/getter (1.0 = 100%)
    void SetZoom(float z);
    float GetZoom() const { return m_zoom; }

    // Apply transform to Graphics (world -> screen).
    // This method sets interpolation and pixel offset for pixel-perfect drawing.
    void ApplyTransform(Gdiplus::Graphics& g);

    // Reset transform and restore high-quality settings for HUD
    static void ResetTransform(Gdiplus::Graphics& g, int screenW, int screenH);

    // Convert coordinates using current camera transform:
    PointF WorldToScreen(const PointF& world) const { return m_coords.WorldToScreen(world); }
    PointF ScreenToWorld(const PointF& screen) const { return m_coords.ScreenToWorld(screen); }

    // Optional: restrict camera to world bounds (world coords)
    void SetWorldBounds(float left, float top, float right, float bottom);

private:
    CoordinateSystem m_coords;
    Entity* m_target;
    int m_viewW, m_viewH;
    float m_zoom;
    float m_smoothing; // linear lerp factor (0..1)
    // world bounds
    bool m_hasBounds;
    float m_left, m_top, m_right, m_bottom;

    // internal helper: clamp offset to bounds
    void ClampOffsetToBounds();
};

