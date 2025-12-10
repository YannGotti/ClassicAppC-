#pragma once
#include <windows.h>       // Всегда первым!
#include <objidl.h>        // Для интерфейсов COM
#include <gdiplus.h>       // GDI+ заголовок
#pragma comment (lib,"Gdiplus.lib")

using namespace std;
using namespace Gdiplus;

#include "Entity.h"

class IAttack
{
protected:
    bool isPunch = false;
    bool isCulldown = false;
    bool isCollide = false;
    int tick = 0;
    int culldown = 100;
    int onePunch = 20;

    bool isMonster = false;
    float monsterCooldown = 2.3f;
    float monsterTimer = 0.0f;

public:
    virtual ~IAttack() = default;

    void UpdateCulldown();
    void MonsterAttackTick(Entity* owner, Entity* target, float dt);
    void LogicAttack(Entity* owner);
    void DebugAttack(Graphics& g);


protected:
    virtual void Attack(Entity* owner, Entity* target);
    virtual bool CanAttack(Entity* owner); 
    virtual bool GetAttackKey();           
};

