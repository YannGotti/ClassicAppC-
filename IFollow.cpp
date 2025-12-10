#include "IFollow.h"
#include "EntityController.h"
#include <iostream>
#include <Windows.h>
using namespace std;

void IFollow::Update(Entity* self, float deltaTime)
{
    if (!target) target = EntityController::GetPlayer();
    if (!target)
    {
        self->velocityX = 0;
        self->velocityY = 0;
        self->SetState(State::Idle);
        return;
    }

    float deltaX = target->x - self->x;
    float deltaY = target->y - self->y;
    float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

    if (distance <= attackRange)
    {
        self->velocityX = 0;
        self->velocityY = 0;

        self->wantAttack = true;

        self->SetState(State::Idle);
        return;
    }
    else
    {
        self->wantAttack = false; 
    }

    // Монстр не двигается если игрок слишком далеко или слишком близко
    if (distance > detectionRadius || distance <= attackRange)
    {
        self->velocityX = 0;
        self->velocityY = 0;
        self->SetState(State::Idle);
        return;
    }

    // Правильное нормализованное направление движения
    float dirX = deltaX / distance;
    float dirY = deltaY / distance;

    self->velocityX = dirX * self->speed;
    self->velocityY = dirY * self->speed;

    // Направление спрайта
    Direction newDir = (fabs(deltaX) > fabs(deltaY))
        ? (deltaX > 0 ? Direction::Right : Direction::Left)
        : (deltaY > 0 ? Direction::Down : Direction::Up);

    if (self->GetDirection() != newDir)
        self->SetDirection(newDir);

    if (self->GetState() != State::Walking)
        self->SetState(State::Walking);

    // Debug
    std::wstring msg = L"Monster Debug | velX:" + std::to_wstring(self->velocityX) +
        L" velY:" + std::to_wstring(self->velocityY) +
        L" Dist:" + std::to_wstring(distance) + L"\n";
    //OutputDebugString(msg.c_str());
}

void IFollow::DebugIFollow(Graphics& g)
{
	FontFamily fontFamily(L"Arial");
	Font font(&fontFamily, 14, FontStyleRegular, UnitPixel);
	SolidBrush brush(Color(255, 255, 0, 0));

    wstring debugText = L"";

	g.DrawString(
		debugText.c_str(),
		-1,
		&font,
		PointF((float)(10), 10),
		&brush
	);
}
