#include "Camera.h"
#include <cmath>

Camera::Camera()
{
    x = 0.0f;
    y = 0.0f;
    zoom = 1.0f;
    smoothing = 0.9f;
}

// move camera toward target (target is world coordinates, e.g., player's center)
void Camera::FollowTarget(float targetWorldX, float targetWorldY)
{
    // simple linear interpolation (lerp). smoothing is how fast we approach:
    // larger smoothing -> faster; we clamp to avoid nan.
    float lerp = smoothing;
    x = x + (targetWorldX - x) * lerp;
    y = y + (targetWorldY - y) * lerp;
}

// Apply GDI+ transform: we want world point (x,y) to land at screen center.
// Matrix constructor: Matrix(m11, m12, m21, m22, dx, dy)
// For uniform scale: m11 = zoom, m22 = zoom, dx = screenCenterX - x*zoom
// To achieve pixel-perfect, round dx/dy to nearest integer.
void Camera::ApplyTransform(Gdiplus::Graphics& g, int screenW, int screenH)
{
    using namespace Gdiplus;

    float screenCX = screenW * 0.5f;
    float screenCY = screenH * 0.5f;

    // compute translation such that camera center maps to screen center after scaling
    float dx = screenCX - x * zoom;
    float dy = screenCY - y * zoom;

    // Pixel-perfect hack: round translation to integer pixels (after scaling),
    // this avoids sub-pixel offset blurring when using NearestNeighbor.
    float rdx = std::round(dx);
    float rdy = std::round(dy);

    // Configure graphics for pixel-perfect drawing
    g.SetInterpolationMode(InterpolationModeNearestNeighbor);
    g.SetPixelOffsetMode(PixelOffsetModeNone); // we align pixels manually
    g.SetSmoothingMode(SmoothingModeNone);

    // create and set matrix: (zoom 0 0 zoom dx dy)
    Matrix m(zoom, 0.0f, 0.0f, zoom, rdx, rdy);
    g.SetTransform(&m);
}

void Camera::ResetTransform(Gdiplus::Graphics& g)
{
    using namespace Gdiplus;
    g.ResetTransform();
    // For HUD we want text rendering high-quality
    g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
    g.SetSmoothingMode(SmoothingModeHighQuality);
}
