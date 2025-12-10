// IAttack.cpp
#include "IAttack.h"
#include <windows.h>
#include "CollisionManager.h"

void IAttack::UpdateCulldown()
{
    if (!isCulldown) return;

    if (tick > onePunch) {
        isPunch = false;
        isCollide = false;
    }

    if (tick < culldown) {
        tick++;
    }
    else {
        tick = 0;
        isCulldown = false;
    }
}

bool IAttack::CanAttack(Entity* owner)
{
    return (owner->dx == 0 && owner->dy == 0);
}

bool IAttack::GetAttackKey()
{
    return GetAsyncKeyState(VK_LBUTTON) & 0x8000;
}

void IAttack::LogicAttack(Entity* owner)
{
    if (!CanAttack(owner)) return;

    if (isPunch && isCulldown) {
        owner->SetState(State::Attacking); 
        return;
    }

    if (isCulldown) return;
    if (!GetAttackKey()) return;

    isPunch = true;
    isCulldown = true;
    owner->SetState(State::Attacking);

    Entity* target = CollisionManager::CollideEntity(owner);
    isCollide = (target != nullptr);

    if (isCollide) {
        Attack(owner, target);
    }
}

void IAttack::MonsterAttackTick(Entity* owner, Entity* target, float dt)
{
    if (monsterTimer > 0.0f)
    {
        monsterTimer -= dt;
        return;
    }

    Attack(owner, target);

    monsterTimer = monsterCooldown;

    owner->ForceState(State::Attacking, 0.3f);
}

void IAttack::Attack(Entity* owner, Entity* target)
{
    target->ModifyHealth(false, owner->GetDamage());
    target->ForceState(State::Damage, 0.3f);
}

void IAttack::DebugAttack(Graphics& g)
{
    FontFamily fontFamily(L"Arial");
    Font font(&fontFamily, 14, FontStyleRegular, UnitPixel);
    SolidBrush brush(Color(255, 255, 0, 0));

    std::wstring debugText =
        L"tick: " + std::to_wstring(tick) +
        L" isCuldown: " + std::to_wstring(isCulldown) +
        L" isPunch: " + std::to_wstring(isPunch) +
        L" isCollide: " + std::to_wstring(isCollide);

    g.DrawString(
        debugText.c_str(),
        -1,
        &font,
        PointF(10.0f, 10.0f),
        &brush
    );
}
