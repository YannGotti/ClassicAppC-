 #include "Camera.h"
#include "Entity.h"
#include <algorithm>
#include <cmath>


using namespace Gdiplus;
using namespace std;

Camera::Camera()
	: m_coords(1.0f, PointF(0, 0)),
	m_target(nullptr),
	m_viewW(800), m_viewH(600),
	m_zoom(1.0f),
	m_smoothing(0.8f),
	m_hasBounds(false),
	m_left(0), m_top(0),
	m_right(0), m_bottom(0) {}

void Camera::Follow(Entity* entity) {
	m_target = entity;
}

void Camera::SetViewSize(int width, int height) {
	m_viewW = width;
	m_viewH = height;
}

void Camera::SetSmoothing(float smoothing) {
	m_smoothing = clamp(smoothing, 0.0f, 1.0f);  
}

void Camera::SetZoom(float z) {
	if (z <= 0.0f) return;

	PointF centerWorld =
		m_coords.ScreenToWorld(PointF(m_viewW / 2.0f, m_viewH / 2.0f));
	m_zoom = z;
	m_coords.SetScale(m_zoom);

	PointF newOffset = PointF(centerWorld.X - (m_viewW / (2.0f * m_zoom)),
		centerWorld.Y - (m_viewH / (2.0f * m_zoom)));
	m_coords.SetOffset(newOffset);
	ClampOffsetToBounds();
}

void Camera::ApplyTransform(Graphics& g)
{
	float scale = m_zoom;

	PointF offset = m_coords.GetOffset();

	float dx = -offset.X * scale;
	float dy = -offset.Y * scale;

	float rdx = round(dx);
	float rdy = round(dy);

	g.SetInterpolationMode(InterpolationModeNearestNeighbor);
	g.SetSmoothingMode(SmoothingModeNone);
	g.SetPixelOffsetMode(PixelOffsetModeNone);

	Matrix m(scale, 0.0f, 0.0f, scale, rdx, rdy);
	g.SetTransform(&m);
}

void Camera::ResetTransform(Graphics& g, int sreenW, int screenH)
{
	g.ResetTransform();
	g.SetInterpolationMode(InterpolationModeHighQuality);
	g.SetSmoothingMode(SmoothingModeHighQuality);
	g.SetPixelOffsetMode(PixelOffsetModeHalf);
}

void Camera::SetWorldBounds(float left, float top, float right, float bottom)
{
	m_hasBounds = true;
	m_left = left; m_top = top; m_right = right; m_bottom = bottom;
	ClampOffsetToBounds();
}

void Camera::ClampOffsetToBounds() {
	if (!m_hasBounds) return;

	// минимальный и максимальный offset
	float minOffsetX = m_left;
	float maxOffsetX = std::max(m_right - m_viewW / m_zoom, m_left);

	float minOffsetY = m_top;
	float maxOffsetY = std::max(m_bottom - m_viewH / m_zoom, m_top);

	PointF off = m_coords.GetOffset();

	off.X = clamp(off.X, minOffsetX, maxOffsetX);
	off.Y = clamp(off.Y, minOffsetY, maxOffsetY);

	m_coords.SetOffset(off);
}

void Camera::Update(float /*dt*/) {
	if (!m_target) return;

	PointF targetCenter = m_target->GetCenterF();

	float desiredOffsetX = targetCenter.X - (m_viewW / (2.0 * m_zoom));
	float desiredOffsetY = targetCenter.Y - (m_viewH / (2.0 * m_zoom));
	
	PointF curOffset = m_coords.GetOffset();

	float lerp = m_smoothing;
	float newX = curOffset.X + (desiredOffsetX - curOffset.X) * lerp;
	float newY = curOffset.Y + (desiredOffsetY - curOffset.Y) * lerp;

	m_coords.SetOffset(PointF(newX, newY));

	ClampOffsetToBounds();
}
